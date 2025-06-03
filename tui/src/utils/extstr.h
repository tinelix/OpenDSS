/* String extensions for C++98/C++03.
 *
 * Use this code for your program.
 *
 * Author: Dmitry Tretyakov (tretdm), 2024-05-20
 */

#pragma once

#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

class ExtString {
public:
    static bool strcrlfc(char* out);
    static int strcut(char* str, int begin, int len);
    static int wcscut(wchar_t* str, int begin, int len);
    static int strendq(const char* str, const char* suffix);
    static int wcsendq(const wchar_t* str, const wchar_t* suffix);
    static char* strwrap(char* out, int width);
    static size_t strlines(char* out, bool useCrLf);
    static char** strsplitln(char* out);
};
