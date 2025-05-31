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
