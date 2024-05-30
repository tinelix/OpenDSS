#include "audiodec.h"
#include "stream.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

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
    ma_engine engine;
    ma_sound sound;
    ma_result result;

    ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(
        ma_format_s32,
        getStreamInfo()->channels,
        (sizeof(buffer) / sizeof(char)) / getStreamInfo()->channels,
        buffer,
        NULL);
    bufferConfig.sampleRate = getStreamInfo()->sampleRate;
    ma_audio_buffer audio_buffer;
    ma_audio_buffer_init(&bufferConfig, &audio_buffer);
    result = ma_sound_init_from_data_source(&engine, &audio_buffer,
                                            MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION,
                                            NULL, &sound);
    if (result != MA_SUCCESS)
    {
        return;
    }

    ma_uint64 count;
    ma_sound_get_length_in_pcm_frames(&sound, &count);
    ma_sound_set_looping(&sound, 1);
    ma_sound_start(&sound);
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
