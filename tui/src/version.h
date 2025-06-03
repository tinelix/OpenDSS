/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  ----------------------------------------------------------------------------
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
 *  information, (in the opening comment of each file).
 */

#pragma once

#define OPENDSS_VERSION "0.0.1"

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#endif

class OpenDSSVersion {
public:
    static char* getVersion() {
        char* version = new char[24];
        #ifdef _MSVC2005G
            #ifdef OPENDSS_SUFFIX
                sprintf_s(version, 24, "%s", OPENDSS_VERSION);
            #else
                sprintf_s(version, 24, "%s", OPENDSS_VERSION);
            #endif
        #else
            #ifdef OPENDSS_SUFFIX
                sprintf(version, "%s", OPENDSS_VERSION);
            #else
                sprintf(version, "%s", OPENDSS_VERSION);
            #endif
        #endif

        return version;
    }

    #ifdef _WIN32
        static int* getWindowsVersion() { // Correct getting Windows OS version
            int* version = new int[3];

            version[0] = 0;
			version[1] = 0;
			version[2] = 0;
            
            return version;
        }
    #endif
};
