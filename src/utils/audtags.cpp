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
    id3->id3_read(path, id3_tags);

    sprintf(tags->artist, "%s", id3_tags->artist);
    sprintf(tags->title, "%s", id3_tags->title);
    sprintf(tags->album, "%s", id3_tags->album);
    return tags;
}
