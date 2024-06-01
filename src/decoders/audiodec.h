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

#ifndef OPENDSS_DECODERS_AUDIODEC_H
#define OPENDSS_DECODERS_AUDIODEC_H

#define DECODER_NOT_IMPLEMENTED -1
#define DECODER_INTERNAL_ERROR  -2

#include "stream.h"
#include "../interfaces/audiodec.h"
#include "math.h"
#include <time.h>

typedef struct
{
    float left_phase;
    float right_phase;
    short* buffer;
    int bufferSampleCount;
}
paUserData;

class AudioDecoder {
    public:
        virtual int open(char* pFileName);
        virtual int decode();
        virtual int close();
        virtual int getFramesCount() = 0;
        virtual int getFrameRate() = 0;
        virtual int getFrameWidth() = 0;
        virtual int getErrorNumber() = 0;
        virtual StreamInfo* getStreamInfo();
        void setInterface(IAudioDecoder* pInterface);
        int initOutput();
        void output(short* buffer);
        void output(char* pFileName);
        int getPlaybackPosition();
        int getPlaybackDuration();
        char* gFileName;
    protected:
        unsigned char* gBuffer;
        int gFramesCount, gFrameRate, gSamples;
        bool gOpen;
};

struct AudioDecThreadParams {
    AudioDecoder* audioDec;
    char*         fileName;
    short*         buffer;
};

#endif
