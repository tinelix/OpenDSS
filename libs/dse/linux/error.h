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

typedef struct {
    int err_num;
    int err_cmd;
} DseError;

// Error codes
#define DSE_ERROR_OPEN_PCM                      -1
#define DSE_ERROR_HWPARAMS_ALLOCATION           -2
#define DSE_ERROR_HWPARAMS_SETUP                -3
#define DSE_ERROR_HWPARAMS_ACCESS               -4
#define DSE_ERROR_HWPARAMS_FORMAT               -5

// Commands
#define DSE_COMMAND_PREPARE 0x8001

#endif // DSE_LINUX_ERROR_H
