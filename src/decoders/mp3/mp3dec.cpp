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

// MiniMP3 Decoder usage (cannot be placed in an headers file)

#define DR_MP3_IMPLEMENTATION
#include <dr_mp3.h>

drmp3 gMP3;
drmp3dec_frame_info gMP3info;

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
    if(!drmp3_init_file(&gMP3, pFileName, NULL)) {
        return DECODER_INTERNAL_ERROR;
    }
    gOpen = true;
    return 0;
}

int MP3Decoder::decode() {
    if(!gOpen) {
        return -1;
    }

	drmp3_uint8 *enc_buffer = (drmp3_uint8*)
		malloc(gMP3.dataSize * sizeof(drmp3_uint8));
    drmp3_uint8 *buffer = (drmp3_uint8*)
		malloc(2048 * sizeof(drmp3_uint8));

    gSamples = gMP3.dataSize;
	
	

	int result = drmp3dec_decode_frame(
		&gMP3.decoder, enc_buffer, 1152, buffer, &gMP3info
	);

    /*pthread_t audioDecThread;
    AudioDecThreadParams* params = new AudioDecThreadParams();
    params->audioDec = this;
    params->fileName = gFileName;
    audioDecoderThread((void*)params);*/

	if(gSamples != gMP3.dataSize) {
        return -1;
    } else {
        return 0;
    }
}

int MP3Decoder::getFramesCount() {
    if(!gOpen) {
        return -1;
    }
	return (gMP3.dataSize / gMP3info.channels);
}

int MP3Decoder::getFrameRate() {
    return gMP3info.hz / getFrameWidth();
}

int MP3Decoder::getFrameWidth() {
    return gMP3info.hz >= 32000 ? 1152 : 576;
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
    streamInfo->codec = gMP3info.hz >= 32000 ?
                        STREAMINFO_CODEC_MPEG1_L3 : STREAMINFO_CODEC_MPEG2_L3;
	streamInfo->sampleRate = gMP3info.hz;
	streamInfo->bitrate = gMP3info.bitrate_kbps;
	streamInfo->channels = gMP3info.channels;
	streamInfo->lengthSec = ((double)gMP3.dataSize / gMP3info.hz / gMP3info.channels);
    return streamInfo;
}

int MP3Decoder::getErrorNumber() {
    return 0;
}

