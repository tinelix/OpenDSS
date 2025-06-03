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

#include "audtags.h"

AudioTager::AudioTager() {

}

AudioTager::~AudioTager() {

}

struct AudioTags* AudioTager::readTags(char* path) {
    struct AudioTags* tags = new AudioTags();
    ID3* id3 = new ID3();
    ID3_Tags* id3_tags = new ID3_Tags();

    // Read data from the tag
    id3->id3_read(path, id3_tags, ID3_V2);

    sprintf(tags->artist, "%s", id3_tags->artist);
    sprintf(tags->title, "%s", id3_tags->title);
    sprintf(tags->album, "%s", id3_tags->album);
    return tags;
}
