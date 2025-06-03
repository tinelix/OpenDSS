/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------------
 *  Copyright © 2024, 2025 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License 3 (or any later version)
 *  and/or Apache License 2.
 *
 *  See the following files in repository directory for the precise terms and
 *  conditions of either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing
 *  information (in the opening comment of each file).
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
