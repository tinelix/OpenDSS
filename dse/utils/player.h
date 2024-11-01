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

#ifndef DSE_UTILS_PLAYER_H
#define DSE_UTILS_PLAYER_H

// Decoders
#include <dse/decoders/mpegaudio/mpegaudio.h>

#include "extstr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned int  in_format;
    unsigned int  sample_rate;
    unsigned int  bitrate;
    unsigned int  channels_count;
    unsigned long decoded_frames;
} dse_player_info;

int dse_player_open_src(char path[400]);
float dse_player_get_position(void);
float dse_player_get_duration(void);

#ifdef __cplusplus
}
#endif

#endif // DSE_UTILS_PLAYER_H
