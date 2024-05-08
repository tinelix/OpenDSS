#ifndef OPENDSS_UTILS_EXTSTR_H
#define OPENDSS_UTILS_EXTSTR_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

class ExtString {
    public:
        static int strcut(char *str, int begin, int len);
        static int strendq(const char *str, const char *suffix);
};

#endif
