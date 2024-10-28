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
    #include "windows/dse.h"
#else
    #include <alsa/asoundlib.h>
    #include "linux/dse.h"
#endif

typedef struct {
    char src_path[400];
    DseError *last_error;
} DigitalSoundEngine;

int dse_init(void);
int dse_prepare(void);
int dse_open_src(char path[400]);

#endif // DSE_DSE_H
