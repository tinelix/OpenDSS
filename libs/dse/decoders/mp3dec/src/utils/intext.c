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

#include <utils/intext.h>
#include <stdio.h>

unsigned long b2uint32(const unsigned char* buf, size_t len) {
    unsigned long result = 0;
	size_t i;

    for (i = 0; i < len && i < sizeof(unsigned long); ++i) {
        result |= buf[i] << (8 * i);
    }
    return result;
}

void int16_to_uint8(short* src, size_t len, unsigned char* dest)
{
	size_t i;
    for (i = 0; i < len; ++i) {
        dest[2 * i + 0] = (unsigned char)(src[i]);
        dest[2 * i + 1] = (unsigned char)(src[i] >> 8);
    }
}