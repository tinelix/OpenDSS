#include <utils/intext.h>
#include <stdio.h>

unsigned long b2uint32(const unsigned char* buf, size_t len) {
    unsigned long result = 0;
    for (size_t i = 0; i < len && i < sizeof(unsigned long); ++i) {
        result |= buf[i] << (8 * i);
    }
    return result;
}