/*
 Public Domain (www.unlicense.org)
 This is free and unencumbered software released into the public domain.
 Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
 software, either in source code form or as a compiled binary, for any purpose,
 commercial or non-commercial, and by any means.
 In jurisdictions that recognize copyright laws, the author or authors of this
 software dedicate any and all copyright interest in the software to the public
 domain. We make this dedication for the benefit of the public at large and to
 the detriment of our heirs and successors. We intend this dedication to be an
 overt act of relinquishment in perpetuity of all present and future rights to
 this software under copyright law.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 Original version written by Krister Lagerström(krister@kmlager.com)
 Website: https://sites.google.com/a/kmlager.com/www/projects

 Contributors:
   technosaurus
   Erik Hofman (added a subset of the libmpg123 compatible streaming API)
   Dmitry Tretyakov (ported PDMP3 for DSE)
*/

#ifndef DSE_DECODERS_PDMP3_PDMP3_H
#define DSE_DECODERS_PDMP3_PDMP3_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#ifdef OUTPUT_SOUND
#include <sys/soundcard.h>
#endif

typedef struct { /* Scale factor band indices,for long and short windows */
  unsigned l[23];
  unsigned s[14];
}
t_sf_band_indices;

/** define a subset of a libmpg123 compatible streaming API */
#define PDMP3_OK           0
#define PDMP3_ERR         -1
#define PDMP3_NEED_MORE  -10
#define PDMP3_NEW_FORMAT -11
#define PDMP3_NO_SPACE     7

#define PDMP3_ENC_SIGNED_16 (0x080|0x040|0x10)

#define INBUF_SIZE      (4*4096)
typedef struct
{
  size_t processed;
  unsigned istart,iend,ostart;
  unsigned char in[INBUF_SIZE];
  unsigned out[2][576];
  t_mpeg1_header g_frame_header;
  t_mpeg1_side_info g_side_info;  /* < 100 words */
  t_mpeg1_main_data g_main_data;

  unsigned hsynth_init;
  unsigned synth_init;
  /* Bit reservoir for main data */
  unsigned g_main_data_vec[2*1024];/* Large static data */
  unsigned *g_main_data_ptr;/* Pointer into the reservoir */
  unsigned g_main_data_idx;/* Index into the current byte(0-7) */
  unsigned g_main_data_top;/* Number of bytes in reservoir(0-1024) */
  /* Bit reservoir for side info */
  unsigned side_info_vec[32+4];
  unsigned *side_info_ptr;  /* Pointer into the reservoir */
  unsigned side_info_idx;  /* Index into the current byte(0-7) */

  char new_header;
}
pdmp3_handle;

pdmp3_handle* pdmp3_new(const char *decoder,int *error);
void pdmp3_delete(pdmp3_handle *id);
int pdmp3_open_feed(pdmp3_handle *id);
int pdmp3_feed(pdmp3_handle *id,const unsigned char *in,size_t size);
int pdmp3_read(pdmp3_handle *id,unsigned char *outmemory,size_t outsize,size_t *done);
int pdmp3_decode(pdmp3_handle *id,const unsigned char *in,size_t insize,unsigned char *out,size_t outsize,size_t *done);
int pdmp3_getformat(pdmp3_handle *id,long *rate,int *channels,int *encoding);
/** end of the subset of a libmpg123 compatible streaming API */

void pdmp3(char * const *mp3s);

#ifndef PDMP3_HEADER_ONLY
#define SIM_UNIX
#define TRUE               1
#define FALSE              0
#define C_SYNC             0xfff00000
#define C_EOF              0xffffffff
#define C_PI               3.14159265358979323846
#define C_INV_SQRT_2       0.70710678118654752440
#define Hz                 1
#define kHz                1000*Hz
#define bps                1
#define kbps               1000*bit_s
#define FRAG_SIZE_LN2      0x0011 /* 2^17=128kb */
#define FRAG_NUMS          0x0004

#ifndef NDEBUG
#define DBG(str,args...) { printf(str,## args); printf("\n"); }
#define ERR(str,args...) { fprintf(stderr,str,## args) ; fprintf(stderr,"\n"); }
#else
#define DBG(str,args...)
#define ERR(str,args...)
#endif


#ifdef DEBUG //debug functions
void dmp_fr(t_mpeg1_header *hdr);
void dmp_si(t_mpeg1_header *hdr,t_mpeg1_side_info *si);
void dmp_scf(t_mpeg1_side_info *si,t_mpeg1_main_data *md,int gr,int ch);
void dmp_huff(t_mpeg1_main_data *md,int gr,int ch);
void dmp_samples(t_mpeg1_main_data *md,int gr,int ch,int type);
#else
#define dmp_fr(...) do{}while(0)
#define dmp_si(...) do{}while(0)
#define dmp_scf(...) do{}while(0)
#define dmp_huff(...) do{}while(0)
#define dmp_samples(...) do{}while(0)
#endif
static int Decode_L3(pdmp3_handle *id);
static int Get_Bytes(pdmp3_handle *id,unsigned no_of_bytes,unsigned data_vec[]);
static int Get_Main_Data(pdmp3_handle *id,unsigned main_data_size,unsigned main_data_begin);
static int Huffman_Decode(pdmp3_handle *id,unsigned table_num,int32_t *x,int32_t *y,int32_t *v,int32_t *w);
static int Read_Audio_L3(pdmp3_handle *id);
static int Read_CRC(pdmp3_handle *id);
static int Read_Frame(pdmp3_handle *id);
static int Search_Header(pdmp3_handle *id);
static int Read_Main_L3(pdmp3_handle *id);
static int Set_Main_Pos(pdmp3_handle *id,unsigned bit_pos);

static unsigned Get_Inbuf_Filled(pdmp3_handle *id);
static unsigned Get_Inbuf_Free(pdmp3_handle *id);

static unsigned Get_Byte(pdmp3_handle *id);
static unsigned Get_Main_Bit(pdmp3_handle *id);
static unsigned Get_Main_Bits(pdmp3_handle *id,unsigned number_of_bits);
static unsigned Get_Main_Pos(pdmp3_handle *id);
static unsigned Get_Side_Bits(pdmp3_handle *id,unsigned number_of_bits);
static unsigned Get_Filepos(pdmp3_handle *id);

static void Error(const char *s,int e);
static void Get_Sideinfo(pdmp3_handle *id,unsigned sideinfo_size);
static void IMDCT_Win(float in[18],float out[36],unsigned block_type);
static void L3_Antialias(pdmp3_handle *id,unsigned gr,unsigned ch);
static void L3_Frequency_Inversion(pdmp3_handle *id,unsigned gr,unsigned ch);
static void L3_Hybrid_Synthesis(pdmp3_handle *id,unsigned gr,unsigned ch);
static void L3_Requantize(pdmp3_handle *id,unsigned gr,unsigned ch);
static void L3_Reorder(pdmp3_handle *id,unsigned gr,unsigned ch);
static void L3_Stereo(pdmp3_handle *id,unsigned gr);
static void L3_Subband_Synthesis(pdmp3_handle *id,unsigned gr,unsigned ch,unsigned outdata[576]);
static void Read_Huffman(pdmp3_handle *id,unsigned part_2_start,unsigned gr,unsigned ch);
static void Requantize_Process_Long(pdmp3_handle *id,unsigned gr,unsigned ch,unsigned is_pos,unsigned sfb);
static void Requantize_Process_Short(pdmp3_handle *id,unsigned gr,unsigned ch,unsigned is_pos,unsigned sfb,unsigned win);
static void Stereo_Process_Intensity_Long(pdmp3_handle *id,unsigned gr,unsigned sfb);
static void Stereo_Process_Intensity_Short(pdmp3_handle *id,unsigned gr,unsigned sfb);

#endif // DSE_DECODERS_PDMP3_PDMP3_H
