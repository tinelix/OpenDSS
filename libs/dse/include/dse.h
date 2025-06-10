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

#ifndef DSE_DSE_H
#define DSE_DSE_H

#ifdef _WIN32
    #include "../include/platform/windows/dsewin.h"
    #include "../include/platform/windows/errcodes.h"
    #include <windows.h>
#else
    #include <alsa/asoundlib.h>
    #include "../include/platform/linux/dse.h"
    #include "../include/platform/linux/errcodes.h"
#endif



#define STD_SAMPLE_RATE 44100
#define STD_BUFFER_SIZE 44100 * sizeof(short)

unsigned char *dse_audio_buffer;
char* dse_file_path;

LIB_EXPORT int dse_init();
LIB_EXPORT int dse_prepare();
LIB_EXPORT int dse_open_src(char path[260]);

#endif // DSE_DSE_H
