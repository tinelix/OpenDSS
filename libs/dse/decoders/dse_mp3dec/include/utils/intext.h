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

#include <stdio.h>

#ifndef _MP3DEC_UTILITIES_INTEXT_H
#define _MP3DEC_UTILITIES_INTEXT_H

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define PI 3.14159265358979323846

unsigned long b2uint32(const unsigned char* buf, size_t len);
void int16_to_uint8(short* src, size_t len, unsigned char* dest);

#endif