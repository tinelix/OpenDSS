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

#include <utils/player.h>

int result;

#ifdef __cplusplus
extern "C" {
#endif

int dse_player_open_src(char path[512]) {
    if(strendq(path, ".mp3") > 0) {
        // TODO: Write MPEG Audio decoder as DLL library
    }
    return result;
}

float dse_player_get_position(void) {
    return 0;
}

float dse_player_get_duration(void) {
    return 0;
}

#ifdef __cplusplus
}
#endif
