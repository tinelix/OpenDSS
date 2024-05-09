/*  Copyright (c) 2006-2014, Philip Busch <vzxwco@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ID3_H
#define _ID3_H

// GCC fixes
#include <cstddef>
#include <cstdio>

#define TAG_LENGTH 128

#define SIZE_INFO   30
#define SIZE_YEAR    4
#define SIZE_GENRE   1
#define SIZE_TRACK   1

#define OFFSET_TITLE 3
#define OFFSET_ARTIST 33
#define OFFSET_ALBUM 63
#define OFFSET_YEAR 93
#define OFFSET_COMMENT 97
#define OFFSET_TRACK 126
#define OFFSET_GENRE 127

#define GENRE_MAX 147

static const char *genres[] = {
	"Blues",		"Classic Rock",			"Country",
	"Dance",		"Disco",                        "Funk",
	"Grunge",		"Hip-Hop",			"Jazz",
	"Metal",		"New Age",			"Oldies",
	"Other",		"Pop",				"R&B",
	"Rap",			"Reggae",			"Rock",
	"Techno",		"Industrial",			"Alternative",
	"Ska",			"Death Metal",			"Pranks",
	"Soundtrack",		"Euro-Techno",                  "Ambient",
	"Trip-Hop",             "Vocal",                        "Jazz+Funk",
	"Fusion",		"Trance",                       "Classical",
	"Instrumental",		"Acid",                         "House",
	"Game",			"Sound Clip",			"Gospel",
	"Noise",		"Alternative Rock",		"Bass",
	"Punk",			"Space",                        "Meditative",
	"Instrumental Pop",	"Instrumental Rock",            "Ethnic",
	"Gothic",		"Darkwave",			"Techno-Industrial",
	"Electronic",		"Pop-Folk",                     "Eurodance",
	"Dream",		"Southern Rock",                "Comedy",
	"Cult",			"Gangsta",                      "Top 40",
	"Christian Rap",	"Pop/Funk",                     "Jungle",
	"Native US",		"Cabaret",                      "New Wave",
	"Psychedelic",		"Rave",                         "Showtunes",
	"Trailer",		"Lo-Fi",                        "Tribal",
	"Acid Punk",		"Acid Jazz",                    "Polka",
	"Retro",		"Musical",                      "Rock & Roll",
	"Hard Rock",		"Folk",                         "Folk-Rock",
	"National Folk",	"Swing",                        "Fast Fusion",
	"Bebob",		"Latin",                        "Revival",
	"Celtic",		"Bluegrass",			"Avantgarde",
	"Gothic Rock",		"Progressive Rock",             "Psychedelic Rock",
	"Symphonic Rock",	"Slow Rock",			"Big Band",
	"Chorus",		"Easy Listening",		"Acoustic",
	"Humour",		"Speech",			"Chanson",
	"Opera",		"Chamber Music",		"Sonata",
	"Symphony",		"Booty Bass",                   "Primus",
	"Porn Groove",		"Satire",                       "Slow Jam",
	"Club",			"Tango",                        "Samba",
	"Folklore",		"Ballad",                       "Power Ballad",
	"Rhythmic Soul",	"Freestyle",                    "Duet",
	"Punk Rock",		"Drum Solo",			"Acappella",
	"Euro-House",		"Dance Hall",                   "Goa",
	"Drum & Bass",		"Club-House",			"Hardcore",
	"Terror",		"Indie",			"BritPop",
	"Negerpunk",		"Polsk Punk",                   "Beat",
	"Christian Gangsta",	"Heavy Metal",                  "Black Metal",
	"Crossover",		"Contemporary Christian",	"Christian Rock",
	"Merengue",		"Salsa",			"Thrash Metal",
	"Anime",		"JPop",				"SynthPop"
};

struct ID3_Tags {
	char title[31];
	char artist[31];
	char album[31];
	char year[5];
	char comment[31];
	unsigned char track;
	unsigned char genre;
};

// C++ class

class ID3 {
	public:
		int id3_read(const char *_path, ID3_Tags *_id3);
		int id3_write(const char *_path, ID3_Tags *_id3);
		void id3_print(ID3_Tags *_id3);
		int id3_read_tag(const char *_path, char *_tag);
		int id3_write_tag(FILE *_fp, ID3_Tags *_id3);
		int id3_check_tag(const char *_tag);
		int id3_extract_info(const char *_tag, ID3_Tags *_id3);
		void id3_normalize_info(ID3_Tags *_id3);
		void id3_get_genre_as_string(ID3_Tags *_id3, char *_genre);
		static void remove_trailing_whitespaces(char *_str);
		static void write_with_padding(FILE *_fp, const char *_str, size_t _len);
};

#endif /* ! _ID3_H */
