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

#ifndef DSE_LINUX_DSE_H
#define DSE_LINUX_DSE_H

#include <dse/error.h>
#include "error.h"

#include <alsa/asoundlib.h>

#ifdef __cplusplus
extern "C" {
#endif

int dse_alsa_init(void);
int dse_alsa_prepare_hardware(void);
int dse_alsa_prepare_buffer(int format, int sample_rate, int channels);
int dse_alsa_output(
    unsigned char data
);

#ifdef __cplusplus
}
#endif

#endif // DSE_LINUX_DSE_H
