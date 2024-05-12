#ifndef _HEXARR_H
#define _HEXARR_H

#include <cstdlib>
#include <stdint.h>

class HexCharArray {
public:
    static uint32_t toDec(unsigned char *bytes, int size, int offset);
};

#endif
