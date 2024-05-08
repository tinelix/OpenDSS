#include <id3v2lib.h>

#include "audtags.h"

AudioTager::AudioTager() {

}

AudioTager::~AudioTager() {

}

struct AudioTags* AudioTager::readTags(char path[255]) {
    struct AudioTags* tags = new AudioTags();
    ID3v2_tag* tag = ID3v2_read_tag(path);

    if(tag == NULL) {
        tag = ID3v2_Tag_new();
    }

    // Read data from the tag
    ID3v2_frame* artist_frame = ID3v2_Tag_get_artist(tag);
    tags->artist = artist_frame->text;

    ID3v2_frame* title_frame = ID3v2_Tag_get_title(tag);
    tags->title = title_frame->text;

    ID3v2_frame* album_frame = ID3v2_Tag_get_album(tag);
    tags->album = album_frame->text;
}
