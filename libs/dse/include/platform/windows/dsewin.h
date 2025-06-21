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
#include "../../utils/stream.h"
#include <windows.h>
#include <dse.h>

typedef struct {
    char path[400];
    dse_error *last_error;
} dse_source;

typedef struct {
    int sample_rate;
    int bits_per_sample;
	int channels;
} DSE_AUDIO_OUTPUT_INFO;

typedef struct {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
} DSE_WAVE_OUTPUT_FORMAT;

typedef struct {
	int		 buffer_size;
	WAVEHDR	 *header;
	WAVEHDR  *next_header;
	int		 finished;
	int		 playing;
} DSE_USER_DATA;

DSE_AUDIO_OUTPUT_INFO out_info;

int dse_win32_init();

int dse_win32_prepare(DSE_AUDIO_OUTPUT_INFO out);
void dse_win32_allocate_frame();
int dse_win32_decode_frame();
void dse_win32_play();
void dse_win32_free_frame();

int dse_win32_open_src(char path[260]);
double dse_win32_calculate_rms_u8(
	const unsigned char *samples, int samples_size
);
double dse_win32_calculate_rms_s16le(
	const unsigned char *samples, int samples_size
);

int dse_win32_get_stream_info(DSE_STREAM_INFO* info);
int dse_win32_is_eof();
int dse_win32_close_input();

#endif // DSE_WINDOWS_DSE_H
