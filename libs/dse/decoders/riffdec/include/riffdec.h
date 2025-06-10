/*  RIFF/WAVE decoder for OpenDSE (Open Digital Sound Engine)
 *  -----------------------------------------------------------------------------
 *  Published as public domain.
 */

#ifndef _RIFFDEC_H
#define _RIFFDEC_H

#include <stdio.h>
#include <errno.h>

#define CHUNK_SIZE 4096
#define MAX_PATH_LENGTH 512

#pragma pack(push, 1)
typedef struct {

    unsigned char	chunk_id[4];
    unsigned long	chunk_size;
    unsigned char	format[4];
    
    unsigned char	subchunk1_id[4];
    unsigned long	subchunk1_size;
    unsigned short	audio_fmt;
    unsigned short	channels;
    unsigned long	sample_rate;
    unsigned long	byte_rate;
    unsigned short	block_align;
    unsigned short	bits_per_sample;
    
    unsigned char	subchunk2_id[4];
    unsigned long	subchunk2_size;

} RIFF_WAVE_HEADER;

typedef struct {
	int sample_rate;
	int channels;
	int bits_per_sample;
} DSE_AUDIO_OUTPUT_INFO;

#pragma pack(pop)

#endif