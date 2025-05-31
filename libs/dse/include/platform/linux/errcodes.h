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

#ifndef DSE_LINUX_ERROR_H
#define DSE_LINUX_ERROR_H

// Error codes
#define DSE_ERROR_ALSA_CANNOT_OPEN_PCM               0xFE01
#define DSE_ERROR_ALSA_HWPARAMS_ALLOCATION_FAILED    0xFE02
#define DSE_ERROR_ALSA_HWPARAMS_SETUP_FAILED         0xFE03
#define DSE_ERROR_ALSA_FAILED_TO_PREPARE_HWPARAMS    0xFE04

#endif // DSE_LINUX_ERROR_H
