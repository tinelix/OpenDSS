#ifndef OPENDSS_DECODERS_MP3_MP3DEC_H
#define OPENDSS_DECODERS_MP3_MP3DEC_H

#include "../audiodec.h"
#include "../stream.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>

class MP3Decoder : AudioDecoder {
public:
    MP3Decoder();
    ~MP3Decoder();
    int open(char* pFileName);
    int decode();
    int getFramesCount();
    int getFrameRate();
    int getFrameWidth();
    int getErrorNumber();
    StreamInfo* getStreamInfo();
};

#endif
