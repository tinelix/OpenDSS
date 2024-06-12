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

#include "mp3dec.h"

IAudioDecoder* gInterface;

// MiniMP3 Decoder usage (cannot be placed in an headers file)

#define DR_MP3_IMPLEMENTATION
#include <dr_mp3.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

drmp3 gMP3Dec;
ma_result gMAResult;
ma_device_config gDeviceConfig;
ma_device gDevice;

static void audioCallback(
    ma_device* pDevice, void* pFramesOut, const void* pFramesIn, ma_uint32 frameCount
);

// Main functions

MP3Decoder::MP3Decoder() {
    gFrameRate = 0;
    gFramesCount = 0;
    gSamples = 0;
}

MP3Decoder::~MP3Decoder() {

}

static void* audioDecoderThread(void* arg) {
    AudioDecThreadParams *params = (AudioDecThreadParams*) arg;

    params->audioDec->output(params->fileName);

    return 0;
}

void MP3Decoder::setInterface(IAudioDecoder* pInterface) {
    gInterface = pInterface;
}

int MP3Decoder::open(char* pFileName) {
    gFileName = pFileName;

    if (!drmp3_init_file(&gMP3Dec, pFileName, NULL)) {
        return DECODER_INTERNAL_ERROR;
    }

    gOpen = true;
    return 0;
}

int MP3Decoder::decode() {
    if(!gOpen) {
        return -1;
    }

    AudioDecThreadParams* params = new AudioDecThreadParams();
    params->audioDec = this;
    params->fileName = gFileName;

    gDeviceConfig = ma_device_config_init(ma_device_type_playback);
    gDeviceConfig.playback.format = ma_format_s16;
    gDeviceConfig.playback.channels = gMP3Dec.channels;
    gDeviceConfig.sampleRate = gMP3Dec.sampleRate;
    gDeviceConfig.dataCallback = audioCallback;
    gDeviceConfig.pUserData = &gMP3Dec;

    gMAResult = ma_device_init(NULL, &gDeviceConfig, &gDevice);
    if (gMAResult != MA_SUCCESS) {
        drmp3_uninit(&gMP3Dec);
        return -2;
    }

    gMAResult = ma_device_start(&gDevice);
    if (gMAResult != MA_SUCCESS) {
        ma_device_uninit(&gDevice);
        drmp3_uninit(&gMP3Dec);
        return -3;
    }

    return 0;
}

int MP3Decoder::getFramesCount() {
    if(!gOpen) {
        return -1;
    }
    return 0;
}

int MP3Decoder::getFrameRate() {
    return gMP3Dec.sampleRate / getFrameWidth();
}

int MP3Decoder::getFrameWidth() {
    return gMP3Dec.sampleRate >= 32000 ? 1152 : 576;
}

StreamInfo* MP3Decoder::getStreamInfo() {
    StreamInfo* streamInfo = new StreamInfo();
    if(!gOpen) {
        streamInfo->codec = 0;
        streamInfo->sampleRate = 0;
        streamInfo->bitrate = 0;
        streamInfo->channels = 0;
        streamInfo->lengthSec = 0;
        return streamInfo;
    }

    return streamInfo;
}

int MP3Decoder::getErrorNumber() {
    return -1;
}

static void audioCallback(
    ma_device* pDevice, void* pFramesOut, const void* pFramesIn, ma_uint32 frameCount
) {

    int lRMS, rRMS;

    drmp3* pMP3Dec;

    AudioSpectrum* gSpectrum;
    StreamTimestamp* gStreamTs;

    pMP3Dec = (drmp3*)pDevice->pUserData;
    DRMP3_ASSERT(pMP3Dec != NULL);

    if (pDevice->playback.format == ma_format_f32) {
        drmp3_read_pcm_frames_f32(
            pMP3Dec, frameCount, (float*)pFramesOut
        );
    }
    else if (pDevice->playback.format == ma_format_s16) {
        drmp3_read_pcm_frames_s16(
            pMP3Dec, frameCount, (drmp3_int16*)pFramesOut
        );
    }
    else {
        DRMP3_ASSERT(DRMP3_FALSE);  /* Should never get here. */
    }

    gStreamTs = new StreamTimestamp();
    gSpectrum = new AudioSpectrum();

    short int* buffer = (short int*)pFramesOut;
    size_t bufferSize = sizeof(buffer) / (int)sizeof(short int);

    double* multiChRMS = (double*)malloc(pMP3Dec->channels * sizeof(double));

    short int** multiChBuffer = AudioDecoder::splitAudioBuffer(
        buffer, bufferSize,
        ma_get_bytes_per_sample(pDevice->playback.format),
        pMP3Dec->channels
    );


    for (int i = 0; i < pMP3Dec->channels; i++) {
        double rms = AudioDecoder::getRMS(multiChBuffer[i], bufferSize / pMP3Dec->channels);
        multiChRMS[i] = rms;
    }

    gSpectrum->left = multiChRMS[0] * 100;
    if (pMP3Dec->channels >= 2)
        gSpectrum->right = multiChRMS[1] * 100;

    if (gSpectrum->left > 100) {
        gSpectrum->left = 100;
    }

    if (gSpectrum->right > 100) {
        gSpectrum->right = 100;
    }

    gInterface->onStreamClock(gSpectrum, gStreamTs);

    free(multiChRMS);
    free(multiChBuffer);

    (void)pFramesIn;
}


