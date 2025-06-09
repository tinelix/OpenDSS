/*  MPEG Layer III decoder for OpenDSE (Open Digital Sound Engine)
 *  -----------------------------------------------------------------------------
 *  Published as public domain.
 *
 *  NOTE: The source code was partially written by AI and was later adapted
 *        and improved by the developer, as it simplifies the process of
 *        developing complex media encoding algorithms.
 *
 *        Despite these improvements, the code may contain errors due to the
 *        lack of comprehensive documentation on working with compressed
 *        audio data in the C language.
 */

#ifndef _MP3DEC_STREAM_H
#define _MP3DEC_STREAM_H

#include <stdio.h>

#define SIZE_OF_SIDE_INFO_BYTES   32
#define SFB_COUNT                 22
#define SPECTRAL_LINES           576
#define TIME_DOMAIN_POINTS      1152
#define AVG_FRAME_SIZE           172  
#define MAX_PCM_FRAME_SIZE      2304

typedef struct {
    unsigned char version           : 2;
    unsigned char layer             : 2;
    unsigned char crc_b             : 1;
    unsigned char br_idx            : 4;
    unsigned char sr_idx            : 2;
    unsigned char padding_b         : 1;
    unsigned char private_b         : 1;
    unsigned char mode              : 2;
    unsigned char mode_ext          : 2;
    unsigned char copyright_b       : 1;
    unsigned char original_b        : 1;
    unsigned char emphasis          : 2;
} MPEG_ES_HEADER;

static const int MPEG_L3_BITRATE_TABLE[][16] = {
    // MPEG-1 Layer III
    {
        0,  32,  40,  48,  56,  64,  80,  96,
           112, 128, 160, 192, 224, 256, 320
    },
    // MPEG-2 Layer III
    {
        0,  8,  16,  24,  32,  40,  48,  56,
           64,  80,  96, 112, 128, 144, 160
    }
};

static const int MPEG_ES_SR_TABLE[][3] = {
    {44100, 48000, 32000},              // MPEG-1
    {22050, 24000, 16000}               // MPEG-2
};

typedef struct {
    unsigned char   blk_type;           // block type (long/short)
    unsigned char   mdb;                // begin of main data
    unsigned char   sfc;                // scalefac compression
    unsigned char   sfb;                // scalefac bands amounts
    unsigned char   sfl[4][21];         // scalefac of left channel 
    unsigned char   sfr[4][21];         // scalefac of right channel 
    unsigned int    sfo;                // scalefac offset
    int             intens_stereo;      // intensity stereo?
    int             ms_stereo;          // ms stereo?
} MPEG_L3_SIDEINFO;

enum MPEG_L3_REGION_TYPE {
    REGION_SHORT = 0,
    REGION_LONG = 1
};

typedef struct {
    int start[SFB_COUNT];       // start of band
    int end[SFB_COUNT];         // end of band
    int grt;                    // granule region type
    int values[SPECTRAL_LINES];
} MPEG_L3_FREQBAND;

int dse_mp3dec_read_header(
    MPEG_ES_HEADER* header,
    const unsigned char* in_buf,
    const size_t buf_size
);

void dse_mp3dec_sideinfo_decode(
    const unsigned char* data, MPEG_L3_SIDEINFO* si
);

void dse_mp3dec_bypass_id3v2(
    const unsigned char* buf, size_t buf_size
);

#endif