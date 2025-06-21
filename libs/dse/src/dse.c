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

LIB_EXPORT int dse_prepare(DSE_AUDIO_OUTPUT_INFO out) {

    #ifdef _WIN32
        result = dse_win32_prepare(out);
    #endif

    return result;
}

LIB_EXPORT int dse_open_input(const char path[512]) {
    dse_file_path = path;
    #ifdef _WIN32
        result = dse_win32_open_input(dse_file_path);
    #endif

    return result;
}

LIB_EXPORT int dse_allocate_frame() {
    #ifdef _WIN32
        dse_win32_allocate_frame();
    #endif

    return result;
}

LIB_EXPORT int dse_decode_frame() {
    #ifdef _WIN32
        result = dse_win32_decode_frame();
    #endif

    return result;
}

LIB_EXPORT void dse_play() {
    #ifdef _WIN32
        dse_win32_play();
    #endif
}

LIB_EXPORT int dse_is_eof() {
    #ifdef _WIN32
        result = dse_win32_is_eof();
    #endif

	return result;
}


LIB_EXPORT double dse_get_frame_rms() {

	double rms;

	#ifdef _WIN32
		rms = dse_win32_get_frame_rms();
	#endif

	return rms;
}

LIB_EXPORT int dse_get_stream_info(DSE_STREAM_INFO* info) {

	#ifdef _WIN32
		return dse_win32_get_stream_info(info);
	#endif
}

LIB_EXPORT int dse_free_frame() {
    #ifdef _WIN32
        dse_win32_free_frame();
    #endif

    return result;
}

LIB_EXPORT int dse_close_input(const char path[512]) {
    dse_file_path = path;
#ifdef _WIN32
    result = dse_win32_close_input(dse_file_path);
#endif

    return result;
}

#ifdef __cplusplus
}
#endif
