/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU General Public License 3 (or any later version) and/or Apache License 2
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */

#pragma once

#define OPENDSS_VERSION "0.0.1"

#ifdef LICENSE_GPLV3
#define OPENDSS_SUFFIX "libre"
#endif

#include <stdio.h>

class OpenDSSVersion {
public:
    static char* getVersion() {
        char* version = new char[24];
        #ifdef _MSVC
            #ifdef OPENDSS_SUFFIX
                sprintf_s(version, strlen(version), "%s-%s", OPENDSS_VERSION, OPENDSS_SUFFIX);
            #else
                sprintf_s(version, strlen(version), "%s", OPENDSS_VERSION);
            #endif
        #else
            #ifdef OPENDSS_SUFFIX
                sprintf(version, "%s-%s", OPENDSS_VERSION, OPENDSS_SUFFIX);
            #else
                sprintf(version, "%s", OPENDSS_VERSION);
            #endif
        #endif

        return version;
    }
};