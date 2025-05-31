#ifndef _MP3DEC_MPEGES_STREAM_H
#define _MP3DEC_MPEGES_STREAM_H

#include <utils/intext.h>

#define GET_BITS(value, start_bit, end_bit) \
   (((b2uint32((value), sizeof(unsigned long))) >> \
    (start_bit)) & ((1UL << ((end_bit)-(start_bit)+1))-1))
#define MIN(a,b) ((a)<(b)?(a):(b))

// Maximum frame size for MPEG Layer I/II/III
#define MAX_FRAME_SIZE 1792

typedef struct {
    unsigned short sample_rate;
    unsigned short bits_per_sample;
    unsigned short channel_mode;
} DSE_AUDIO_OUTPUT_INFO;

typedef enum {
    LAYER_I = 3,
    LAYER_II = 2,
    LAYER_III = 1
} MP3DEC_LAYERS;

typedef struct {
    int valid;          // Vaild frame or not?
    int sample_rate;    // Sample rate
    int channels;       // Channels count
    int frame_size;     // Frame size
} MP3DEC_FRAME_INFO;

typedef struct {
    //  Type         | Key               | Length  | Description
    unsigned long  sync                    : 12;  // Sync word              (12 bits)
    unsigned long  id                      :  1;  // MPEG version           (1 bit)
    unsigned long  layer                   :  2;  // Audio coding profile   (2 bits)
    unsigned long  protection_bit          :  1;  // CRC                    (1 bit)
    unsigned long  bitrate_index           :  4;  // Bitrate index          (4 bits)
    unsigned long  sri                     :  2;  // Sample rate index      (2 bits)
    unsigned long  padding_bit             :  1;  // Padding                (1 bit)
    unsigned long  private_bit             :  1;  // Private?               (1 bit)
    unsigned long  mode                    :  2;  // Channel mode           (2 bits)
    unsigned long  mode_extension          :  2;  // Mode extension         (2 bits)
    unsigned long  copyright               :  1;  // Copyright bit          (1 bit)
    unsigned long  original                :  1;  // Original?              (1 bit)
    unsigned long  emphasis                :  2;  // Emphasis value         (2 bits)
} MP3DEC_FRAME_HEADER;

static const unsigned char dse_mp3dec_sample_rate_table[] = {
    /* MPEG-1 */            44100, 48000, 32000,
    /* MPEG-2 */            22050, 24000, 16000,
    /* MP3+ */              11025, 12000, 8000
};

int dse_mp3dec_decode_frame(
    const unsigned char* input,
    size_t frame_len,
    unsigned char* pcm_buf,
    size_t pcm_buf_size
);

#endif
