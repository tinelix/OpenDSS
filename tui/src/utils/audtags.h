/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  ----------------------------------------------------------------------------
 *  Copyright © 2024, 2025 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License 3 (or any later version) 
 *  and/or Apache License 2.
 *
 *  See the following files in repository directory for the precise terms and 
 *  conditions of either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing 
 *  information, (in the opening comment of each file).
 */

#ifndef OPENDSS_UTILS_AUDTAGS_H
#define OPENDSS_UTILS_AUDTAGS_H

extern "C" {
    #include "../../libs/libid3/src/id3.h"
}

#include <cstdio>

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
        struct AudioTags *readTags(char *path);
};

#endif
