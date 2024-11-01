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

#ifndef DSE_ERROR_H
#define DSE_ERROR_H

typedef struct {
    int err_num;
    int err_cmd;
} dse_error;

// Common DSE error codes
#define DSE_ERROR_OUTPUT_DEVICE_NOT_AVALIABLE       0xFD01
#define DSE_ERROR_INVALID_OUTPUT_FORMAT             0xFD02
#define DSE_ERROR_INVALID_INPUT_FORMAT              0xFD03
#define DSE_ERROR_INVALID_OUTPUT_SAMPLE_RATE        0xFD04
#define DSE_ERROR_INVALID_INPUT_SAMPLE_RATE         0xFD05
#define DSE_ERROR_INVALID_OUTPUT_CHANNELS_COUNT     0xFD06
#define DSE_ERROR_INVALID_INPUT_CHANNELS_COUNT      0xFD07
#define DSE_ERROR_FAILED_TO_PREPARE_PCM             0xFD08
#define DSE_ERROR_FAILED_TO_WRITE_PCM               0xFD09
#define DSE_ERROR_FILE_NOT_FOUND                    0xFD0A
#define DSE_ERROR_INTERNAL                          0xFD0B

// Commands
#define DSE_COMMAND_PREPARE                         0x8001
#define DSE_COMMAND_OPEN_INPUT                      0x8002
#define DSE_COMMAND_DECODE                          0x8003

#endif // DSE_ERROR_H
