/*  Digital Sound Engine - basis of Tinelix OpenDSS
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU Lesser General Public License 2.1 (or any later version) and/or BSD 3-Clause
 *  License.
 *
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.BSD
 *     LICENSE.LGPL
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */

#ifndef DSE_WINDOWS_DSE_H
#define DSE_WINDOWS_DSE_H

#define LIB_EXPORT __declspec(dllexport)
#define LIB_IMPORT __declspec(dllimport)

#include "../../utils/errcodes.h"
#include <windows.h>

typedef struct {
    char path[400];
    dse_error *last_error;
} dse_source;

typedef struct {
    int bits_per_sample;
    int channels;
    int sample_rate;
} DSE_PCM_OUTPUT_FORMAT;

typedef struct {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
} DSE_WAVE_OUTPUT_FORMAT;

int dse_win32_init(void);
int dse_win32_prepare(DSE_PCM_OUTPUT_FORMAT out);
int dse_win32_open_src(char path[260]);
int dse_win32_close_input();

#endif // DSE_WINDOWS_DSE_H
