#include "audiodec.h"
#include "stream.h"

int AudioDecoder::open(char* pFileName) {
    gFileName = pFileName;
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

    return 0;
}

void AudioDecoder::output(unsigned char* buffer) {
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
