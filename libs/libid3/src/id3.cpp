/*  Copyright (c) 2006-2014, Philip Busch <vzxwco@gmail.com>
 *  Copyright (c) 2024, Dmitry Tretyakov <tinelix@mail.ru>
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

#include <cstdio>
#include <cstring>
#include <assert.h>
#include "hexarr.h"
#include "id3.h"

int ID3::id3_read(const char *path, ID3_Tags *id3, int id3ver)
{
	char* tmp;

	if(id3_read_tag(path, tmp, id3, id3ver) < 0)
	        return(-1);

	id3_normalize_info(id3);

	free(tmp);

	return(0);
}

int ID3::id3_write(const char *path, ID3_Tags *id3, int id3ver)
{
	ID3_Tags tmp;
	FILE *fp;

	if((fp = fopen(path, "r+")) == NULL)
	        return(-1);

	if(id3_read(path, &tmp, id3ver) == 0) {
		if(fseek(fp, -1 * gTagLength, SEEK_END) == -1)
		        return(-1);
	} else {
		if(fseek(fp, 0, SEEK_END) == -1)
		        return(-1);
	}

	id3_write_tag(fp, id3);

	fclose(fp);

	return(0);
}

void ID3::id3_print(ID3_Tags *id3)
{
	char genre[1024];

	assert(id3 != NULL);

	id3_get_genre_as_string(id3, genre);

	printf("Title..: %s\n", id3->title);
	printf("Artist.: %s\n", id3->artist);
	printf("Album..: %s\n", id3->album);
	printf("Year...: %s\n", id3->year);
	printf("Comment: %s\n", id3->comment);
	printf("Genre..: %s (%d)\n", genre, id3->genre);
	printf("Track..: %d\n", id3->track);
}

int ID3::id3_read_tag(const char *path, char *tag, ID3_Tags *id3, int id3ver)
{
	unsigned char header[10];
	FILE *fp;
	int i;

	if((fp = fopen(path, "r")) == NULL)
		return(-1);

	if(id3ver == ID3_V1) { // ID3v1, ID3v2 or ID3v3?
		if(fseek(fp, -1 * ID3_V1_TAG_LENGTH, SEEK_END) == -1)
			return(-1);

		gTagLength = ID3_V1_TAG_LENGTH;
	} else {
		if(fseek(fp, 0, SEEK_SET) == -1)
			return(-1);
	}

	for(i = 0; i < 10; i++) {
		header[i] = fgetc(fp);
	}

	if(id3ver == ID3_V2) {
		gTagLength = HexCharArray::toDec(header, 4, 6);
	}

	tag = new char[gTagLength];

	if(id3ver == ID3_V2) {
		if(fseek(fp, 12, SEEK_SET) == -1)
			return(-1);
	}

	for(i = 0; i < gTagLength; i++) {
		tag[i] = fgetc(fp);
	}

	fclose(fp);

	if(id3_check_header(header, id3ver) < 0) {
		return(-1);
	}

	if(id3_extract_info(tag, id3, id3ver) < 0)
		return(-1);

	return(0);
}

int ID3::id3_write_tag(FILE *fp, ID3_Tags *id3)
{
	assert(fp != NULL);
	assert(id3 != NULL);

	fprintf(fp, "TAG");
	ID3::write_with_padding(fp, id3->title, ID3_V1_SIZE_INFO);
	ID3::write_with_padding(fp, id3->artist, ID3_V1_SIZE_INFO);
	ID3::write_with_padding(fp, id3->album, ID3_V1_SIZE_INFO);
	ID3::write_with_padding(fp, id3->year, SIZE_YEAR);
	ID3::write_with_padding(fp, id3->comment, ID3_V1_SIZE_INFO);

	if(id3->track != 0) {
		fseek(fp, -1, SEEK_CUR);
		fprintf(fp, "%c", id3->track);
	}

	fprintf(fp, "%c",   id3->genre);

	return(0);
}

int ID3::id3_check_header(unsigned char *header, int id3ver)
{
	assert(header != NULL);

	if(id3ver == ID3_V1) { // ID3v1, ID3v2 or ID3v3?
		if((header[0] != 'T') || (header[1] != 'A') || (header[2] != 'G'))
	        return(-1);
		gTagLength = ID3_V1_TAG_LENGTH;
	} else {
		if((header[0] != 'I') || (header[1] != 'D') || (header[2] != '3'))
	        return(-1);

		// Getting ID3 version
		gID3ver[0] = header[3];
		gID3ver[1] = header[4];

		gTagLength = HexCharArray::toDec(header, 4, 6);
	}

	return(0);
}


int ID3::id3_extract_info(char *tag, ID3_Tags *id3, int id3ver)
{
	assert(tag != NULL);
	assert(id3 != NULL);

	if(id3ver == ID3_V1) {
		memcpy(id3->title,   tag + ID3_V1_OFFSET_TITLE,   ID3_V1_SIZE_INFO);
		memcpy(id3->artist,  tag + ID3_V1_OFFSET_ARTIST,  ID3_V1_SIZE_INFO);
		memcpy(id3->album,   tag + ID3_V1_OFFSET_ALBUM,   ID3_V1_SIZE_INFO);
		memcpy(id3->year,    tag + ID3_V1_OFFSET_YEAR,    SIZE_YEAR);
		memcpy(id3->comment, tag + ID3_V1_OFFSET_COMMENT, ID3_V1_SIZE_INFO);

		id3->track = (tag[ID3_V1_OFFSET_TRACK] == '\0')?0:tag[ID3_V1_OFFSET_TRACK];
		id3->genre = tag[ID3_V1_OFFSET_GENRE];

		id3->title[30]   = '\0';
		id3->artist[30]  = '\0';
		id3->album[30]   = '\0';
		id3->year[4]     = '\0';
		id3->comment[30] = '\0';
	} else {
		memcpy(id3->year,    tag + ID3_V2_OFFSET_YEAR,    SIZE_YEAR);

		id3->track = (tag[ID3_V2_OFFSET_TRACK] == '\0')?0:tag[ID3_V2_OFFSET_TRACK];
		id3->genre = tag[ID3_V2_OFFSET_GENRE];

		int tagOffset = ID3_V2_OFFSET_TITLE;
		int tagArtistOffset = ID3_V2_OFFSET_TITLE;
		int tagAlbumOffset = ID3_V2_OFFSET_TITLE;
		int tagCommentOffset = ID3_V2_OFFSET_TITLE;
		int tagTrackOffset = ID3_V2_OFFSET_TITLE;

		memcpy(id3->title, tag + ID3_V2_OFFSET_TITLE, ID3_V2_SIZE_INFO);

		for(int i; i < ID3_V2_SIZE_INFO; i++) {

			if(id3->title[i] == 'T' && id3->title[i + 1] == 'P'
				&& id3->title[i + 2] == 'E' && id3->title[i + 3] == '1') {
					id3->title[i] = '\0';
				tagArtistOffset += strlen(id3->title) + 11;
				tagOffset = tagArtistOffset;
				memcpy(id3->artist, tag + tagOffset, ID3_V2_SIZE_INFO);
			}

			if(tag[i] == 'T' && tag[i + 1] == 'A'
				&& tag[i + 2] == 'L' && tag[i + 3] == 'B') {
				tagAlbumOffset += i + 2;
				if(tagOffset == tagArtistOffset) {
					id3->artist[i - tagOffset] = '\0';
				}
				tagOffset = tagAlbumOffset;
				memcpy(id3->album, tag + tagOffset, ID3_V2_SIZE_INFO);
			}

			if(tag[i] == 'C' && tag[i + 1] == 'O'
				&& tag[i + 2] == 'M' && tag[i + 3] == 'M') {
				tagCommentOffset += i + 2;
				if(tagOffset == tagAlbumOffset) {
					id3->album[i - tagOffset] = '\0';
				} else if(tagOffset == tagArtistOffset) {
					id3->artist[i - tagOffset] = '\0';
				}
				tagOffset = tagCommentOffset;
				memcpy(id3->album, tag + tagOffset, ID3_V2_SIZE_INFO);
			}


		}
	}

	return(0);
}

void ID3::remove_trailing_whitespaces(char *str)
{
	int i, size;
	
	assert(str != NULL);

	if(str == NULL)
	        return;
	
	size = strlen(str);
	
	for(i = size - 1; i >= 0; i--) {
		if(str[i] == ' ') {
			str[i] = '\0';
		} else break;
	}
}

void ID3::id3_normalize_info(ID3_Tags *id3)
{
	assert(id3 != NULL);

	ID3::remove_trailing_whitespaces(id3->title);
	ID3::remove_trailing_whitespaces(id3->artist);
	ID3::remove_trailing_whitespaces(id3->album);
	ID3::remove_trailing_whitespaces(id3->comment);
}

void ID3::id3_get_genre_as_string(ID3_Tags *id3, char *genre)
{
	assert(id3 != NULL);
	assert(genre != NULL);

	if(id3->genre > GENRE_MAX) {
		strcpy(genre, "Unknown");
	} else {
		strcpy(genre, genres[id3->genre]);
	}
}

void ID3::write_with_padding(FILE *fp, const char *str, size_t len)
{
	unsigned int i;

	assert(fp != NULL);
	assert(str != NULL);

	fprintf(fp, "%s", str);

	for(i = 0; i < len - strlen(str); i++) {
		fprintf(fp, "%c", '\0');
	}
}

// int main()
// {
// 	ID3* id3 = new ID3();
// 	struct ID3_Tags tmp;
//
// 	char path[256];
//
// 	printf("Type a MP3 file path: ");
// 	scanf("%255[^\n]", path);
// 	printf("\r\n");
//
// 	id3->id3_read(path, &tmp, ID3_V2);
// 	id3->id3_print(&tmp);
//
// 	return(0);
// }
