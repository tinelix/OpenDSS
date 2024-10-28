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

DigitalSoundEngine *dse;
int result = -1;

int dse_mci_init(void) {

    if(result >= 0) {
        dse = new DigitalSoundEngine();
    }

    return result;
}

int dse_mci_prepare(void) {
    if(result < 0) {
        snd_pcm_hw_params_free(hwparams);
        snd_pcm_close(handle);
        return result;
    }

    return result;
}

int dse_mci_open_src(char path[400]) {
    return 0;
}
