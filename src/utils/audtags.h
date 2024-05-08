#ifndef OPENDSS_UTILS_AUDTAGS_H
#define OPENDSS_UTILS_AUDTAGS_H

#include <id3v2lib.h>

// Audio formats
#define TAGS_AUDIO_MP3      0x9000
#define TAGS_AUDIO_AAC      0x9001
#define TAGS_AUDIO_WMA      0x9002
#define TAGS_AUDIO_FLAC     0x9003
#define TAGS_AUDIO_ALAC     0x9004
#define TAGS_AUDIO_PCM      0x9005

// Audio channels
#define TAGS_AUDIO_MONO     1
#define TAGS_AUDIO_STEREO   2
#define TAGS_AUDIO_5_1      6
#define TAGS_AUDIO_7_1      8

struct AudioTags {
    char title[160];
    char artist[160];
    char album[160];
    int format;
    int sampleRate;
    long bitrate;
    int channels;
};

class AudioTager {
    public:
        AudioTager();
        ~AudioTager();
        struct AudioTags *readTags(char path[255]);
};

#endif
