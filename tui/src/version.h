/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This file is part of Tinelix OpenDSS.
 *
 *  Tinelix OpenDSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Tinelix OpenDSS is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Tinelix OpenDSS. If not, see <https://www.gnu.org/licenses/>.
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
