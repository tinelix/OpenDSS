#include <mp3dec.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <mpeges/stream.h>

LIB_EXPORT int DSE_OpenMediaSource(const void* data, size_t szData) {

	MP3DEC_FRAME_INFO frame_info;
	DSE_AUDIO_OUTPUT_INFO out_info;

	// Validate data
	if (!data || szData == 0) return NULL;

	if (!dse_mp3dec_parse_frame_header(
		data, &frame_info,
		&out_info
	)) {
		return -1; // Invalid file format
	}
	return 0;
}

LIB_EXPORT int DSE_GetModuleType(void) {
	return 1;
}

LIB_EXPORT const char* DSE_GetSupportedMediaFormats(void) {
	return "*.mp1|*.mp2|*.mp2a|*.mp3|*.mpg";
}

#ifdef __cplusplus
}
#endif

