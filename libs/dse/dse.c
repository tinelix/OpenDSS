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

#include "dse.h"

#ifdef __cplusplus
extern "C" {
#endif

int result = 0;

int dse_init(void) {

    #ifdef _WIN32

    #else
        result = dse_alsa_init();
    #endif

    return result;
}

int dse_prepare(void) {

    #ifdef _WIN32

    #else
        result = dse_alsa_prepare();
    #endif

    return result;
}

int dse_open_src(char path[400]) {
    return 0;
}

#ifdef __cplusplus
}
#endif
