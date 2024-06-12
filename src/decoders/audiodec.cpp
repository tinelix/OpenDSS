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

#ifdef SUPPORT_MODULE_RAUDIO
    #include <raudio.h>
#else
    #include <miniaudio.h>
#endif

bool isPlaying, initializedDevice;

#ifdef SUPPORT_MODULE_RAUDIO
    Music gMusic;
#else

#endif

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
        #ifdef SUPPORT_MODULE_RAUDIO
            InitAudioDevice();
        #endif
        initializedDevice = true;
    }

    return 0;
}

/*   From MusicStream to output
 *   
 *   BUG: Sound remains choppy on Windows.
*/

void AudioDecoder::output(char* pFileName) {
    StreamInfo* streamInfo = getStreamInfo();

}

void AudioDecoder::output(short* buffer) {

}

int AudioDecoder::getPlaybackPosition() {
    return 0;
}

int AudioDecoder::getPlaybackDuration() {
    return 0;
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
    pubInterface = pInterface;
}

short int** AudioDecoder::splitAudioBuffer(
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

double AudioDecoder::getRMS(short int *buffer, int length)
{
    double sumSquared = 0;
    double scaleShortToDouble = 1.0/0x8000;

    for (int i = 0 ; i < length; i++)
    {
         double s = buffer[i] * scaleShortToDouble;
         sumSquared += s * s;
    }
    return sqrt((double)(2)) * sqrt((double)(sumSquared/length));
}

void AudioDecoder::pause() {
    isPlaying = !isPlaying;
    if (!isPlaying)
        #ifdef SUPPORT_MODULE_RAUDIO
            PauseMusicStream(gMusic);
        #else
            int result = -1;
        #endif
    else {
        int result = -1;
    }
}

void AudioDecoder::stop() {
    isPlaying = false;
}

void AudioDecoder::freeStream() {
    if(initializedDevice == true) {
   
    }
}

