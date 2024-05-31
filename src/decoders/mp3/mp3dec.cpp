#include "mp3dec.h"

// MiniMP3 Decoder usage (cannot be placed in an headers file)

#define MINIMP3_IMPLEMENTATION
#include <minimp3.h>
#include <minimp3_ex.h>

mp3dec_t gMP3Dec;
mp3dec_ex_t gMP3ExDec;
mp3dec_file_info_t gMP3Info;

// Main functions

MP3Decoder::MP3Decoder() {
    gFrameRate = 0;
    gFramesCount = 0;
    gSamples = 0;
}

MP3Decoder::~MP3Decoder() {

}

static void *audioDecoderThread(void *arg) {
    AudioDecThreadParams *params = (AudioDecThreadParams*) arg;

    params->audioDec->initOutput();
    params->audioDec->output(params->fileName);
}

int MP3Decoder::open(char* pFileName) {
    gFileName = pFileName;
    if(mp3dec_load(&gMP3Dec, pFileName, &gMP3Info, NULL, NULL)) {
        return DECODER_INTERNAL_ERROR;
    }
    free(gMP3Info.buffer);
    if(mp3dec_ex_open(&gMP3ExDec, pFileName, MP3D_SEEK_TO_SAMPLE)) {
        return DECODER_INTERNAL_ERROR;
    }
    gOpen = true;
    return 0;
}

int MP3Decoder::decode() {
    if(!gOpen) {
        return -1;
    }
    mp3d_sample_t *buffer = (mp3d_sample_t*)
                            malloc(gMP3ExDec.samples * sizeof(mp3d_sample_t));

    gSamples = mp3dec_ex_read(&gMP3ExDec, buffer, gMP3ExDec.samples);

    pthread_t audioDecThread;
    AudioDecThreadParams* params = new AudioDecThreadParams();
    params->audioDec = this;
    params->fileName = gFileName;
    params->buffer = (short*)buffer;
    audioDecoderThread((void*)params);

    if(gSamples != gMP3ExDec.samples && gMP3ExDec.last_error) {
        return -1;
    } else {
        return 0;
    }
}

int MP3Decoder::getFramesCount() {
    if(!gOpen) {
        return -1;
    }
    return (gMP3ExDec.samples / gMP3ExDec.info.channels);
}

int MP3Decoder::getFrameRate() {
    return gMP3ExDec.info.hz / getFrameWidth();
}

int MP3Decoder::getFrameWidth() {
    return gMP3ExDec.info.hz >= 32000 ? 1152 : 576;
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
    streamInfo->codec = gMP3ExDec.info.hz >= 32000 ?
                        STREAMINFO_CODEC_MPEG1_L3 : STREAMINFO_CODEC_MPEG2_L3;
    streamInfo->sampleRate = gMP3ExDec.info.hz;
    streamInfo->bitrate = gMP3ExDec.info.bitrate_kbps;
    streamInfo->channels = gMP3ExDec.info.channels;
    streamInfo->lengthSec = ((double)gMP3ExDec.samples / gMP3ExDec.info.hz / gMP3ExDec.info.channels);
    return streamInfo;
}

int MP3Decoder::getErrorNumber() {
    return gMP3ExDec.last_error ? gMP3ExDec.last_error : 0;
}

