/*  MPEG Layer III decoder for OpenDSE (Open Digital Sound Engine)
 *  -----------------------------------------------------------------------------
 *  Published as public domain.
 *
 *  NOTE: The source code was partially written by AI and was later adapted
 *        and improved by the developer, as it simplifies the process of
 *        developing complex media encoding algorithms.
 *
 *        Despite these improvements, the code may contain errors due to the
 *        lack of comprehensive documentation on working with compressed
 *        audio data in the C language.
 */

#include <mp3dec.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

FILE* _in_f;
unsigned char* _in_buf = NULL;
short* frame_buf = NULL;
int _in_bytes_left = MAX_BLOCK_SIZE;
int _in_bytes_read = 0;
int frame_buf_size = 0;


LIB_EXPORT int DSE_OpenInputSource(const void* data, size_t szData) {

	DSE_AUDIO_OUTPUT_INFO out_info;
	
	return 0;
}

LIB_EXPORT int DSE_OpenInputFile(const char f_path[512]) {

    MPEG_ES_HEADER mpeges;
	size_t bytes_read;
	int result;

    #ifdef _MSVC2005G
        fopen_s(&_in_f, f_path, "rb");
    #else
        _in_f = fopen(f_path, "rb");
    #endif // _MSVC2005G

    if (!_in_f) {
        return -1;
    }

    _in_buf = (unsigned char*) malloc((MAX_BLOCK_SIZE * sizeof(unsigned char)) + 1);
    frame_buf_size = MAX_PCM_FRAME_SIZE;
    frame_buf = (short*) malloc(MAX_PCM_FRAME_SIZE);

    bytes_read = fread(_in_buf, 1, MAX_BLOCK_SIZE, _in_f);

    if (bytes_read <= 0) {
        fclose(_in_f);
        return -2;
    }

    dse_mp3dec_bypass_id3v2(&_in_buf, MAX_BLOCK_SIZE);

    result = dse_mp3dec_read_header(&mpeges, _in_buf, MAX_BLOCK_SIZE);

    if (result < 0) {
        free(_in_f);
        _in_f = NULL;
    }

    return result;
}

LIB_EXPORT int DSE_GetModuleType(void) {
	return 1;
}

LIB_EXPORT const char* DSE_GetSupportedMediaFormats(void) {
	return "*.mp1|*.mp2|*.mp2a|*.mp3|*.mpg";
}

LIB_EXPORT int DSE_DecodeFrameU8(unsigned char** pcm_buf, size_t pcm_len) {

    return -101;
}

LIB_EXPORT int DSE_DecodeFrameS16LE(short** pcm_buf, size_t pcm_len) {

    MPEG_ES_HEADER mpeges;
    MPEG_L3_SIDEINFO si;
    MPEG_L3_FREQBAND spectrum;
    float scale_factors[SFB_COUNT];
    const unsigned int* huff_table = NULL;
    int current_offset;
    double time_domain_output[TIME_DOMAIN_POINTS], normalized_value;
	size_t addit_bytes;
	int line, i;

    if (pcm_len > frame_buf_size) {
        frame_buf_size = pcm_len;
        realloc(frame_buf, pcm_len);
    }

    if (!frame_buf) {
        frame_buf_size = 0;
        frame_buf = NULL;
        return -1;
    }

    if (_in_bytes_left < 72 && _in_f && !feof(_in_f)) {
        memmove(_in_buf, _in_buf + _in_bytes_read, _in_bytes_left);
        addit_bytes = fread(
            _in_buf + _in_bytes_left, 1, MAX_BLOCK_SIZE - _in_bytes_left, _in_f
        );
        _in_bytes_left += addit_bytes;
        _in_bytes_read = 0;
    }

    while (_in_bytes_left > 0) {

        int res = dse_mp3dec_read_header(
            &mpeges, _in_buf, MAX_BLOCK_SIZE
        );

        si.mdb = 128;
        si.sfo = 36;

        if (res < 0) {
            _in_bytes_read += si.mdb;
            _in_bytes_left -= si.mdb;
            _in_buf += si.mdb;
            return res;
        }

        _in_bytes_read += 4;
        _in_bytes_left -= 4;

        res = dse_mp3dec_descale(
            _in_buf + 4, &spectrum, &si
        );

        if (res < 0) {
            _in_bytes_read += si.mdb;
            _in_bytes_left -= si.mdb;
            _in_buf += 800;
            return -2;
        }

        memset(&spectrum, 0, sizeof(MPEG_L3_FREQBAND));

        huff_table = dse_mp3dec_huffman_select(&si);

        if (!huff_table) {
            _in_bytes_read += si.mdb;
            _in_bytes_left -= si.mdb;
            _in_buf += 800;
            return -3;
        }

        current_offset = si.mdb;

        for (line = 0; line < si.sfb; line++) {

            // Huffman data extraction fails
            int decoded_coeff = dse_mp3dec_huffman_decode(
                _in_buf, &current_offset, huff_table
            );
            if (decoded_coeff < 0) {
                _in_bytes_read += si.mdb;
                _in_bytes_left -= si.mdb;
                _in_buf += 800;
                return -4;
            }
            spectrum.values[line] = decoded_coeff;
        }

        dse_mp3dec_imdct_decode(
            spectrum.values, time_domain_output, SFB_COUNT
        );

        for (i = 0; i < pcm_len; i++) {
            normalized_value = time_domain_output[i] * 32767.0f;

            if (normalized_value > 32767) normalized_value = 32767;
            if (normalized_value < -32768) normalized_value = -32768;

            frame_buf[i] = (short)(normalized_value);
        }

        _in_bytes_read += si.mdb;
        _in_bytes_left -= si.mdb;
    }

    pcm_buf = &frame_buf;

    return pcm_len * sizeof(short);
}

LIB_EXPORT int DSE_CloseInputFile() {
    fclose(_in_f);
    return 0;
}

#ifdef __cplusplus
}
#endif

