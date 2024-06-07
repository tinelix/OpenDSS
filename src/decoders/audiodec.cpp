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

#include "audiodec.h"
#include "stream.h"

#include <cstddef>
#include <cstdio>
//#include <raudio.h>

bool isPlaying, initializedDevice;
StreamTimestamp* gStreamTs;
AudioSpectrum* gSpectrum;
IAudioDecoder* gInterface;
/*Music gMusic;*/
int visualizerCalcCount;

static void audioCallback(
    void *bufferData, unsigned int frames
);

int AudioDecoder::open(char* pFileName) {
    gFileName = pFileName;
    visualizerCalcCount = 0;
    gFrameRate = 0;
    gFramesCount = 0;
    return 0;
}

int AudioDecoder::decode() {
    return DECODER_NOT_IMPLEMENTED;
}

int AudioDecoder::close() {
    return DECODER_NOT_IMPLEMENTED;
}

int AudioDecoder::initOutput() {
    if(!initializedDevice) {
        /*InitAudioDevice();
        SetAudioStreamBufferSizeDefault(512);
        initializedDevice = true;*/
    }

    return 0;
}

void AudioDecoder::output(char* pFileName) {
    /*StreamInfo* streamInfo = getStreamInfo();
    gMusic = LoadMusicStream(pFileName);
    SetMasterVolume(1.0);
    PlayMusicStream(gMusic);
    AttachAudioStreamProcessor(gMusic.stream, audioCallback);
    isPlaying = true;
    while(isPlaying) {
       UpdateMusicStream(gMusic);
    }*/
}

void AudioDecoder::output(short* buffer) {

}

int AudioDecoder::getPlaybackPosition() {
    return /*GetMusicTimePlayed(gMusic)*/ 0;
}

int AudioDecoder::getPlaybackDuration() {
    return /*GetMusicTimeLength(gMusic)*/ 0;
}

StreamInfo* AudioDecoder::getStreamInfo() {
    StreamInfo* streamInfo = new StreamInfo();
    streamInfo->codec = 0;
    streamInfo->sampleRate = 0;
    streamInfo->bitrate = 0;
    streamInfo->channels = 0;
    streamInfo->lengthSec = 0;
    return streamInfo;
}

void AudioDecoder::setInterface(IAudioDecoder* pInterface) {
    gInterface = pInterface;
}

static short int** splitAudioBuffer(
    short int *buffer, int length, int sampleSize, int channels
) {
    int channelSize = length / channels;
    int bufferIndex = 0;
    int bytesPerSample = (sampleSize / channels) / 8;

    short int** multiChannelBuffer = (short int**)malloc(length * sizeof(short int));

    for(int ch = 0; ch < channels; ch++) {
        multiChannelBuffer[ch] = (short int*)malloc((sampleSize / channels) * sizeof(short int));
        for(int s = 0; s < channelSize; s += bytesPerSample) {
            for (int i = 0; i < bytesPerSample; i++) {
                multiChannelBuffer[ch][s + i] = buffer[bufferIndex];
                bufferIndex++;
            }
        }
    }

    return multiChannelBuffer;
}

double getRMS(short int *buffer, int length)
{
    double sumSquared = 0;
    double scaleShortToDouble = 1.0/0x8000;

    for (int i = 0 ; i < length; i++)
    {
         double s = buffer[i] * scaleShortToDouble;
         sumSquared += s * s;
    }
    return sqrt((double)2) * sqrt(sumSquared/length);
}

void AudioDecoder::pause() {
    isPlaying = !isPlaying;
    /*if(!isPlaying)
        PauseMusicStream(gMusic);
    else {
        ResumeMusicStream(gMusic);
        while(isPlaying) {
            UpdateMusicStream(gMusic);
        }
    }*/
}

void AudioDecoder::stop() {
    isPlaying = false;
    //StopMusicStream(gMusic);
}

void AudioDecoder::freeStream() {
    if(initializedDevice == true) {
        /*DetachAudioStreamProcessor(gMusic.stream, audioCallback);
        UnloadMusicStream(gMusic);
        CloseAudioDevice();
        initializedDevice = false;*/
    }
}

static void audioCallback(
    void *bufferData, unsigned int frames
) {
    /*int lRMS, rRMS;

    gStreamTs = new StreamTimestamp();
    gSpectrum = new AudioSpectrum();

    short int* buffer = (short int*)bufferData;
    size_t bufferSize = sizeof(buffer) / (int)sizeof(short int);
    double* multiChRMS = (double*) malloc(gMusic.stream.channels * sizeof(double));

    short int** multiChBuffer = splitAudioBuffer(
        buffer, bufferSize,
        gMusic.stream.sampleSize,
        gMusic.stream.channels
    );


    for(int i = 0; i < gMusic.stream.channels; i++) {
        double rms = getRMS(multiChBuffer[i], bufferSize / gMusic.stream.channels);
        multiChRMS[i] = rms;
    }

    free(multiChBuffer);

    gSpectrum->left = multiChRMS[0] * 80;
    if(gMusic.stream.channels >= 2)
        gSpectrum->right = multiChRMS[1] * 80;

    if(gSpectrum->left > 100) {
        gSpectrum->left = 100;
    }

    if(gSpectrum->right > 100) {
        gSpectrum->right = 100;
    }

    free(multiChRMS);

    if(visualizerCalcCount % 4 == 0) {
        gInterface->onStreamClock(gSpectrum, gStreamTs);
    }

    visualizerCalcCount++;*/
}
