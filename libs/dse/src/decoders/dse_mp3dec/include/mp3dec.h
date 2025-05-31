#ifndef _MP3DEC_MP3DEC_H
#define _MP3DEC_MP3DEC_H

#ifdef _WIN32
#include <platform/windows/mp3decw.h>
#endif

LIB_EXPORT int DSE_OpenMediaSource(const void* media_data, size_t data_size);

LIB_EXPORT const char* DSE_GetSupportedFormats(void);

LIB_EXPORT int DSE_GetModuleType(void);

#endif
