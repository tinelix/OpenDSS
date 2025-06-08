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

#ifndef _MP3DEC_MP3DEC_H
#define _MP3DEC_MP3DEC_H

#ifdef _WIN32
#include <platform/windows/mp3decw.h>
#endif

#include <mpeges/stream.h>


#define MAX_BLOCK_SIZE 4096

#include <stdio.h>

LIB_EXPORT int DSE_OpenInputSource(const void* data, size_t szData);

LIB_EXPORT const char* DSE_GetSupportedFormats(void);

LIB_EXPORT int DSE_GetModuleType(void);

#endif
