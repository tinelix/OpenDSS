/*  RIFF/WAVE decoder for OpenDSE (Open Digital Sound Engine)
 *  -----------------------------------------------------------------------------
 *  Published as public domain.
 */

#include "riffdec.h"

FILE *_in_f = NULL;
RIFF_WAVE_HEADER _wav_h;

int DSE_RIFF_OpenInputFile(const char f_path[MAX_PATH_LENGTH]) {
	if (!f_path || strlen(f_path) >= MAX_PATH_LENGTH) return -1;

    _in_f = fopen(f_path, "rb");
    if (!_in_f) return -1;

    fread(&_wav_h, sizeof(RIFF_WAVE_HEADER), 1, _in_f);

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

    return 0;

}

int DSE_RIFF_DecodeFrameUC(unsigned char** pcm_buf, size_t pcm_len) {
	
	size_t frame_size;

    if (!pcm_buf || !_in_f) return -1;

    *pcm_buf = malloc(CHUNK_SIZE);

    if (*pcm_buf == NULL) return -1;

    frame_size = fread(*pcm_buf, 1, CHUNK_SIZE, _in_f);

    if (frame_size > 0 && frame_size <= CHUNK_SIZE) {
        return frame_size;
    }

    free(*pcm_buf);
    return -1;
}

int DSE_RIFF_CloseInputFile() {
    if (_in_f) {
        fclose(_in_f);
        _in_f = NULL;
    }
    return 0;
}