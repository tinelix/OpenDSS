#include "hexarr.h"

unsigned long HexCharArray::toDec(unsigned char *bytes, int size, int offset) {
    unsigned long result = 0x00;
    for(int i = 0; i < size; i++) {
        result = result << 8;
        result = result | (unsigned char)bytes[offset + i];
    }

    unsigned long a, b, c, d, result2 = 0x0;
    a = result & 0xFF;
    b = (result >> 8) & 0xFF;
    c = (result >> 16) & 0xFF;
    d = (result >> 24) & 0xFF;

    result2 = result2 | a;
    result2 = result2 | (b << 7);
    result2 = result2 | (c << 14);
    result2 = result2 | (d << 21);

    return result2;
}
