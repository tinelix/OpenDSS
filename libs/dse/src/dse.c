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

#include <dse.h>

#ifdef __cplusplus
extern "C" {
#endif

int result = 0;

LIB_EXPORT int dse_init(void) {

    #ifdef _WIN32
        result = dse_win32_init();
    #endif

    return result;
}

LIB_EXPORT int dse_prepare(DSE_PCM_OUTPUT_PARAMS out_params) {

    #ifdef _WIN32
        result = dse_win32_prepare();
    #endif

    return result;
}

LIB_EXPORT int dse_open_src(char path[260]) {
    dse_file_path = path;
    #ifdef _WIN32
        result = dse_win32_open_src(dse_file_path);
    #endif

    return result;
}


#ifdef __cplusplus
}
#endif
