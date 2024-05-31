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
