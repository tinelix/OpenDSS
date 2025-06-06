/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This file is part of Tinelix OpenDSS.
 *
 *  Tinelix OpenDSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Tinelix OpenDSS is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Tinelix OpenDSS. If not, see <https://www.gnu.org/licenses/>.
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
