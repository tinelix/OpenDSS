/*  RIFF/WAVE decoder for OpenDSE (Open Digital Sound Engine)
 *  -----------------------------------------------------------------------------
 *  Published as public domain.
 */

#include "riffdec.h"

FILE *_in_f = NULL;
RIFF_WAVE_HEADER _wav_h;
DSE_AUDIO_OUTPUT_INFO out_info;

int riff_bytes_read = 0;

int DSE_RIFF_OpenInputFile(const char f_path[MAX_PATH_LENGTH]) {

	riff_bytes_read = 0;

	if (!f_path || strlen(f_path) >= MAX_PATH_LENGTH) return -1;

    _in_f = fopen(f_path, "rb");
    if (!_in_f) return -1;

    //fread(&_wav_h, sizeof(RIFF_WAVE_HEADER), 1, _in_f);

	fseek(_in_f, 0, SEEK_SET);

	fread(_wav_h.chunk_id,			4, 1, _in_f); 
	fread(&_wav_h.chunk_size,		4, 1, _in_f);
	fread(_wav_h.format,			4, 1, _in_f); 
	fread(_wav_h.subchunk1_id,		4, 1, _in_f);
	fread(&_wav_h.subchunk1_size,	4, 1, _in_f);
	fread(&_wav_h.audio_fmt,		2, 1, _in_f);
	fread(&_wav_h.channels,			2, 1, _in_f);
	fread(&_wav_h.sample_rate,		4, 1, _in_f);
	fread(&_wav_h.byte_rate,		4, 1, _in_f);
	fread(&_wav_h.block_align,		2, 1, _in_f); 
	fread(&_wav_h.bits_per_sample,	2, 1, _in_f);
	fread(_wav_h.subchunk2_id,		4, 1, _in_f);    
	fread(&_wav_h.subchunk2_size,	4, 1, _in_f);

	out_info.sample_rate	 = _wav_h.sample_rate;
	out_info.bits_per_sample = _wav_h.bits_per_sample;
	out_info.channels	     = _wav_h.channels;

    if (
        memcmp(_wav_h.chunk_id, "RIFF", 4) != 0 ||
        memcmp(_wav_h.format, "WAVE", 4) != 0 ||
        memcmp(_wav_h.subchunk1_id, "fmt ", 4) != 0 ||
        memcmp(_wav_h.subchunk2_id, "data", 4) != 0
    ) {
        fclose(_in_f);
        return -1;
    }

    if (_wav_h.audio_fmt != 1) {
        fclose(_in_f);
        return -2;
    }

    fseek(_in_f, sizeof(RIFF_WAVE_HEADER), SEEK_SET);

	riff_bytes_read = sizeof(RIFF_WAVE_HEADER);

    return 0;

}

int DSE_RIFF_DecodeFrameS16LE(short** pcm_buf, size_t pcm_len) {
	
	size_t frame_size;
	int err;
	char debug[400];

    if (!_in_f) return -1;

	if(pcm_len < CHUNK_SIZE)
		return -2;

	if (*pcm_buf == NULL)
		*pcm_buf = (short *)malloc(pcm_len * sizeof(short));

    if (*pcm_buf == NULL) return -1;

    frame_size = fread(*pcm_buf, 1, pcm_len, _in_f);

    if (frame_size > 0 && frame_size <= pcm_len) {
		riff_bytes_read += frame_size;
        return frame_size;
    }

    return -1;
}

int DSE_RIFF_DecodeFrameU8(unsigned char** pcm_buf, size_t pcm_len) {
	
	size_t frame_size;

    if (!_in_f) return -1;

	if(pcm_len < CHUNK_SIZE)
		return -2;

    if (*pcm_buf == NULL) return -1;

    frame_size = fread(*pcm_buf, 1, pcm_len, _in_f);

    if (frame_size > 0 && frame_size <= pcm_len) {
        return frame_size;
    }

    return -1;
}

int DSE_RIFF_GetOutputInfo(DSE_AUDIO_OUTPUT_INFO* info) {
	*(info) = out_info;
	return info;
}

int DSE_RIFF_CloseInputFile() {
    if (_in_f) {
        fclose(_in_f);
        _in_f = NULL;
    }
    return 0;
}