#ifndef OPENDSS_UTILS_EXTSTR_H
#define OPENDSS_UTILS_EXTSTR_H

#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

class ExtString {
    public:
        static bool strcrlfc(char* out);
        static int strcut(char *str, int begin, int len);
        static int strendq(const char *str, const char *suffix);
        static char* strwrap(char *out, int width);
        static size_t strlines(char* out, bool useCrLf);
        static char** strsplitln(char *out);
};

#endif
