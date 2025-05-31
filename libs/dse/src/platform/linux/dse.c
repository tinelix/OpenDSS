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
int dse_alsa_error = -1;
int dse_alsa_result = 0;

snd_pcm_t *handle;
snd_pcm_hw_params_t *hwparams;
unsigned char *audio_data;

#ifdef __cplusplus
extern "C" {
#endif

int dse_alsa_init(void) {

    error = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

    if(dse_alsa_error >= 0) {
        dse = new DigitalSoundEngine();
        dse_alsa_result = 1;
    } else {
        dse_alsa_result = DSE_ERROR_ALSA_CANNOT_OPEN_PCM;
    }

    return dse_alsa_result;
}

int dse_alsa_prepare_hardware(void) {

    dse_alsa_error = snd_pcm_hw_params_malloc(&hwparams);

    if(dse_alsa_error < 0) {
        dse_alsa_result = DSE_ERROR_ALSA_HWPARAMS_ALLOCATION_FAILED;
        return dse_alsa_result;
    }

    dse_alsa_error = snd_pcm_hw_params_any(handle, hwparams));

    if(dse_alsa_error < 0) {
        dse_alsa_free();
        dse_alsa_result = DSE_ERROR_ALSA_HWPARAMS_SETUP_FAILED;
        return dse_alsa_result;
    }

    dse_alsa_error = snd_pcm_hw_params_set_access(
        handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED
    );

    if (dse_alsa_error < 0) {
        dse_alsa_free();
        dse_alsa_result = DSE_ERROR_OUTPUT_DEVICE_NOT_AVALIABLE;
        return result;
    }

    return dse_alsa_error;
}

int dse_alsa_prepare_buffer(
    int format, int sample_rate, int channels
) {
    int alsa_fmt = SND_PCM_FORMAT_S16_LE;

    switch(format) {
        case DSE_SNDFMT_PCM_U8:
            alsa_fmt = SND_PCM_FORMAT_U8
            break;
        case DSE_SNDFMT_PCM_S8:
            alsa_fmt = SND_PCM_FORMAT_S8
            break;
        case DSE_SNDFMT_PCM_U16_BE:
            alsa_fmt = SND_PCM_FORMAT_U16_BE;
            break;
        case DSE_SNDFMT_PCM_U16_LE:
            alsa_fmt = SND_PCM_FORMAT_U16_LE;
            break;
        case DSE_SNDFMT_PCM_S16_BE:
            alsa_fmt = SND_PCM_FORMAT_S16_BE;
            break;
        case DSE_SNDFMT_PCM_U24_BE:
            alsa_fmt = SND_PCM_FORMAT_U24_BE;
            break;
        case DSE_SNDFMT_PCM_U24_LE:
            alsa_fmt = SND_PCM_FORMAT_U24_LE;
            break;
        case DSE_SNDFMT_PCM_S24_BE:
            alsa_fmt = SND_PCM_FORMAT_S24_BE;
            break;
        case DSE_SNDFMT_PCM_S24_LE:
            alsa_fmt = SND_PCM_FORMAT_S24_LE;
            break;
        case DSE_SNDFMT_PCM_U32_BE:
            alsa_fmt = SND_PCM_FORMAT_U32_BE;
            break;
        case DSE_SNDFMT_PCM_U32_LE:
            alsa_fmt = SND_PCM_FORMAT_U32_LE;
            break;
        case DSE_SNDFMT_PCM_S32_BE:
            alsa_fmt = SND_PCM_FORMAT_S32_BE;
            break;
        case DSE_SNDFMT_PCM_S32_LE:
            alsa_fmt = SND_PCM_FORMAT_S32_LE;
            break;
        case DSE_SNDFMT_PCM_F32_BE:
            alsa_fmt = SND_PCM_FORMAT_F32_BE;
            break;
        case DSE_SNDFMT_PCM_F32_LE:
            alsa_fmt = SND_PCM_FORMAT_F32_LE;
            break;
        default:
            alsa_fmt = SND_PCM_FORMAT_S16_LE;
            break;
    }

    dse_alsa_error = snd_pcm_hw_params_set_format(
        handle, hwparams, SND_PCM_FORMAT_S16_LE
    );

    if (dse_alsa_error < 0) {
        dse_alsa_free();
        dse_alsa_result = DSE_ERROR_INVALID_OUTPUT_FORMAT;
        return dse_alsa_result;
    }

    dse_alsa_error = snd_pcm_hw_params_set_rate_near(
        handle, hwparams, &sample_rate, 0
    )

    if (dse_alsa_error < 0) {
        dse_alsa_free();
        dse_alsa_result = DSE_ERROR_INVALID_OUTPUT_SAMPLE_RATE;
        return dse_alsa_result;
    }

    dse_alsa_error = snd_pcm_hw_params_set_channels(handle, hwparams, channels)

    if(dse_alsa_error < 0) {
        dse_alsa_free();
        dse_alsa_result = DSE_ERROR_INVALID_OUTPUT_CHANNELS_COUNT;
        return dse_alsa_result;
    }

    dse_alsa_error = snd_pcm_hw_params(handle, hwparams)

    if(dse_alsa_error < 0) {
        dse_alsa_free();
        dse_alsa_result = DSE_ERROR_ALSA_HWPARAMS_SETUP_FAILED;
        return dse_alsa_result;
    }

    snd_pcm_hw_params_free(hwparams);

    dse_alsa_error = snd_pcm_prepare(handle);

    if (dse_alsa_error < 0) {
        dse_alsa_free();
        dse_alsa_result = DSE_ERROR_ALSA_FAILED_TO_PREPARE_HWPARAMS;
        return dse_alsa_result;
    }
}

int dse_alsa_output(
    unsigned char *data
) {
    audio_data = data;
    size_t data_size = sizeof(data);
    dse_alsa_error = snd_pcm_writei(handle, data, data_size / 4);
    if (dse_alsa_error < 0) {
        snd_pcm_close(handle);
        return DSE_ERROR_FAILED_TO_WRITE_PCM;
    }
    return 0;
}

int dse_alsa_free(void) {
    if(hwparams)
        snd_pcm_hw_params_free(hwparams);
    if(handle)
        snd_pcm_close(handle);
}

#ifdef __cplusplus
}
#endif
