#ifndef OPENDSS_DECODERS_AUDIODEC_H
#define OPENDSS_DECODERS_AUDIODEC_H

#define DECODER_NOT_IMPLEMENTED -1
#define DECODER_INTERNAL_ERROR  -2

#include "stream.h"


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
    protected:
        char* gFileName;
        unsigned char* gBuffer;
        int gFramesCount, gFrameRate, gSamples;
        bool gOpen;
        int initOutput();
        void output(unsigned char* buffer);
};

#endif
