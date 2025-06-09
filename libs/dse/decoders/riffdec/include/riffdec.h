/*  RIFF/WAVE decoder for OpenDSE (Open Digital Sound Engine)
 *  -----------------------------------------------------------------------------
 *  Published as public domain.
 */

#ifndef _RIFFDEC_H
#define _RIFFDEC_H

#include <stdio.h>

#define CHUNK_SIZE 4096
#define MAX_PATH_LENGTH 512

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
    unsigned long	block_align;
    unsigned long	bits_per_sample;
    
    unsigned char	subchunk2_id[4];
    unsigned long	subchunk2_size;

} RIFF_WAVE_HEADER;

#endif