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
	int bits_per_sample;
	int channels;
} DSE_AUDIO_OUTPUT_INFO;

typedef struct {
	int codec_id;
	int sample_rate;
	int bits_per_sample;
	long bitrate;
	int channels;
	int bitrate_mode; // 0 - CBR, 1 - ABR, 2 - VBR
} DSE_STREAM_INFO;

#define STREAMINFO_CODEC_PCM_U8             0x2000      // or Unsigned PCM 8-bit
#define STREAMINFO_CODEC_PCM_S8             0x2001      // or Signed PCM 8-bit
#define STREAMINFO_CODEC_PCM_S16LE          0x2011      // or Signed PCM 16-bit little-endian
#define STREAMINFO_CODEC_PCM_S24LE          0x2021		// or Signed PCM 24-bit little-endian
#define STREAMINFO_CODEC_PCM_S32LE          0x2031		// or Signed PCM 32-bit little-endian
#define STREAMINFO_CODEC_PCM_S32LE_F        0x2032      // or Signed PCM 32-bit little-endian with floating point

#pragma pack(pop)

#endif