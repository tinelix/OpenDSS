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

#include "pdmp3.h"
#include "include/haffman.h"
#include "include/layer3.h"
#include "include/pdmp3.h"

#ifdef DEBUG
static void dmp_fr(t_mpeg1_header *hdr){
  printf("rate %d,sfreq %d,pad %d,mod %d,modext %d,emph %d\n",
          hdr->bitrate_index,hdr->sampling_frequency,hdr->padding_bit,
          hdr->mode,hdr->mode_extension,hdr->emphasis);
}

static void dmp_si(t_mpeg1_header *hdr,t_mpeg1_side_info *si){
  int nch,ch,gr;

  nch = hdr->mode == mpeg1_mode_single_channel ? 1 : 2;
  printf("main_data_begin %d,priv_bits %d\n",si->main_data_begin,si->private_bits);
  for(ch = 0; ch < nch; ch++) {
    printf("scfsi %d %d %d %d\n",si->scfsi[ch][0],si->scfsi[ch][1],si->scfsi[ch][2],si->scfsi[ch][3]);
    for(gr = 0; gr < 2; gr++) {
      printf("p23l %d,bv %d,gg %d,scfc %d,wsf %d,bt %d\n",
              si->part2_3_length[gr][ch],si->big_values[gr][ch],
              si->global_gain[gr][ch],si->scalefac_compress[gr][ch],
              si->win_switch_flag[gr][ch],si->block_type[gr][ch]);
      if(si->win_switch_flag[gr][ch]) {
        printf("mbf %d,ts1 %d,ts2 %d,sbg1 %d,sbg2 %d,sbg3 %d\n",
                si->mixed_block_flag[gr][ch],si->table_select[gr][ch][0],
                si->table_select[gr][ch][1],si->subblock_gain[gr][ch][0],
                si->subblock_gain[gr][ch][1],si->subblock_gain[gr][ch][2]);
      }else{
        printf("ts1 %d,ts2 %d,ts3 %d\n",si->table_select[gr][ch][0],
                si->table_select[gr][ch][1],si->table_select[gr][ch][2]);
      }
      printf("r0c %d,r1c %d\n",si->region0_count[gr][ch],si->region1_count[gr][ch]);
      printf("pf %d,scfs %d,c1ts %d\n",si->preflag[gr][ch],si->scalefac_scale[gr][ch],si->count1table_select[gr][ch]);
    }
  }
}

static void dmp_scf(t_mpeg1_side_info *si,t_mpeg1_main_data *md,int gr,int ch){
  int sfb,win;

  if((si->win_switch_flag[gr][ch] != 0) &&(si->block_type[gr][ch] == 2)) {
    if(si->mixed_block_flag[gr][ch] != 0) { /* First the long block scalefacs */
      for(sfb = 0; sfb < 8; sfb++)
        printf("scfl%d %d%s",sfb,md->scalefac_l[gr][ch][sfb],(sfb==7)?"\n":",");
      for(sfb = 3; sfb < 12; sfb++) /* And next the short block scalefacs */
        for(win = 0; win < 3; win++)
          printf("scfs%d,%d %d%s",sfb,win,md->scalefac_s[gr][ch][sfb][win],(win==2)?"\n":",");
    }else{                /* Just short blocks */
      for(sfb = 0; sfb < 12; sfb++)
        for(win = 0; win < 3; win++)
          printf("scfs%d,%d %d%s",sfb,win,md->scalefac_s[gr][ch][sfb][win],(win==2)?"\n":",");
    }
  }else for(sfb = 0; sfb < 21; sfb++) /* Just long blocks; scalefacs first */
    printf("scfl%d %d%s",sfb,md->scalefac_l[gr][ch][sfb](sfb == 20)?"\n":",");
}

static void dmp_huff(t_mpeg1_main_data *md,int gr,int ch){
  int i;
  printf("HUFFMAN\n");
  for(i = 0; i < 576; i++) printf("%d: %d\n",i,(int) md->is[gr][ch][i]);
}

static void dmp_samples(t_mpeg1_main_data *md,int gr,int ch,int type){
  int i,val;
  extern double rint(double);

  printf("SAMPLES%d\n",type);
  for(i = 0; i < 576; i++) {
    val =(int) rint(md->is[gr][ch][i] * 32768.0);
    if(val >= 32768) val = 32767;
    if(val < -32768) val = -32768;
    printf("%d: %d\n",i,val);
  }
}
#endif

/**Description: calculates y=x^(4/3) when requantizing samples.
* Parameters: TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static inline float Requantize_Pow_43(unsigned is_pos){
#ifdef POW34_TABLE
  static float powtab34[8207];
  static int init = 0;
  int i;

  if(init == 0) {   /* First time initialization */
    for(i = 0; i < 8207; i++)
      powtab34[i] = pow((float) i,4.0 / 3.0);
    init = 1;
  }
#ifdef DEBUG
  if(is_pos > 8206) {
    ERR("is_pos = %d larger than 8206!",is_pos);
    is_pos = 8206;
  }
#endif /* DEBUG */
  return(powtab34[is_pos]);  /* Done */
#elif defined POW34_ITERATE
  float a4,a2,x,x2,x3,x_next,is_f1,is_f2,is_f3;
  unsigned i;
//static unsigned init = 0;
//static float powtab34[32];
  static float coeff[3] = {-1.030797119e+02,6.319399834e+00,2.395095071e-03};
//if(init == 0) { /* First time initialization */
//  for(i = 0; i < 32; i++) powtab34[i] = pow((float) i,4.0 / 3.0);
//  init = 1;
//}
  /* We use a table for 0<is_pos<32 since they are so common */
  if(is_pos < 32) return(powtab34[is_pos]);
  a2 = is_pos * is_pos;
  a4 = a2 * a2;
  is_f1 =(float) is_pos;
  is_f2 = is_f1 * is_f1;
  is_f3 = is_f1 * is_f2;
  /*  x = coeff[0] + coeff[1]*is_f1 + coeff[2]*is_f2 + coeff[3]*is_f3; */
  x = coeff[0] + coeff[1]*is_f1 + coeff[2]*is_f2;
  for(i = 0; i < 3; i++) {
    x2 = x*x;
    x3 = x*x2;
    x_next =(2*x3 + a4) /(3*x2);
    x = x_next;
  }
  return(x);
#else /* no optimization */
return powf((float)is_pos,4.0f / 3.0f);
#endif /* POW34_TABLE || POW34_ITERATE */
}

/**Description: decodes a layer 3 bitstream into audio samples.
* Parameters: Stream handle,outdata vector.
* Return value: PDMP3_OK or PDMP3_ERR if the frame contains errors.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Decode_L3(pdmp3_handle *id){
  unsigned gr,ch,nch;

  /* Number of channels(1 for mono and 2 for stereo) */
  nch =(id->g_frame_header.mode == mpeg1_mode_single_channel ? 1 : 2);
  for(gr = 0; gr < 2; gr++) {
    for(ch = 0; ch < nch; ch++) {
      dmp_scf(&id->g_side_info,&id->g_main_data,gr,ch); //noop unless debug
      dmp_huff(&id->g_main_data,gr,ch); //noop unless debug
      L3_Requantize(id,gr,ch); /* Requantize samples */
      dmp_samples(&id->g_main_data,gr,ch,0); //noop unless debug
      L3_Reorder(id,gr,ch); /* Reorder short blocks */
    } /* end for(ch... */
    L3_Stereo(id,gr); /* Stereo processing */
    dmp_samples(&id->g_main_data,gr,0,1); //noop unless debug
    dmp_samples(&id->g_main_data,gr,1,1); //noop unless debug
    for(ch = 0; ch < nch; ch++) {
      L3_Antialias(id,gr,ch); /* Antialias */
      dmp_samples(&id->g_main_data,gr,ch,2); //noop unless debug
      L3_Hybrid_Synthesis(id,gr,ch); /*(IMDCT,windowing,overlapp add) */
      L3_Frequency_Inversion(id,gr,ch); /* Frequency inversion */
     dmp_samples(&id->g_main_data,gr,ch,3); //noop unless debug
      L3_Subband_Synthesis(id,gr,ch,id->out[gr]); /* Polyphase subband synthesis */
    } /* end for(ch... */
#ifdef DEBUG
    {
      int i,ctr = 0;
      printf("PCM:\n");
      for(i = 0; i < 576; i++) {
        printf("%d: %d\n",ctr++,(out[i] >> 16) & 0xffff);
        if(nch == 2) printf("%d: %d\n",ctr++,out[i] & 0xffff);
      }
    }
#endif /* DEBUG */
  } /* end for(gr... */
  return(PDMP3_OK);   /* Done */
}

static unsigned Get_Inbuf_Filled(pdmp3_handle *id) {
  return (id->istart<=id->iend)?(id->iend-id->istart):(INBUF_SIZE-id->istart+id->iend);
}

static unsigned Get_Inbuf_Free(pdmp3_handle *id) {
  return  (id->iend<id->istart)?(id->istart-id->iend):(INBUF_SIZE-id->iend+id->istart);
}


/** Description: reads 'no_of_bytes' from input stream into 'data_vec[]'.
*   Parameters: Stream handle,number of bytes to read,vector pointer where to
                store them.
*   Return value: PDMP3_OK or PDMP3_ERR if the operation couldn't be performed.
*   Author: Krister Lagerström(krister@kmlager.com) **/
static int Get_Bytes(pdmp3_handle *id,unsigned no_of_bytes,unsigned data_vec[]){
  int i;
  unsigned val;

  for(i = 0; i < no_of_bytes; i++) {
    val = Get_Byte(id);
    if(val == C_EOF) return(C_EOF);
    else data_vec[i] = val;
  }
  return(PDMP3_OK);
}

/** Description: This function assembles the main data buffer with data from
*              this frame and the previous two frames into a local buffer
*              used by the Get_Main_Bits function.
* Parameters: Stream handle,main_data_begin indicates how many bytes from
*             previous frames that should be used. main_data_size indicates the
*             number of data bytes in this frame.
* Return value: Status
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Get_Main_Data(pdmp3_handle *id,unsigned main_data_size,unsigned main_data_begin){
  int i;

  if(main_data_size > 1500) ERR("main_data_size = %d\n",main_data_size);
  /* Check that there's data available from previous frames if needed */
  if(main_data_begin > id->g_main_data_top) {
    /* No,there is not,so we skip decoding this frame,but we have to
     * read the main_data bits from the bitstream in case they are needed
     * for decoding the next frame. */
   (void) Get_Bytes(id,main_data_size,&(id->g_main_data_vec[id->g_main_data_top]));
    /* Set up pointers */
    id->g_main_data_ptr = &(id->g_main_data_vec[0]);
    id->g_main_data_idx = 0;
    id->g_main_data_top += main_data_size;
    return(PDMP3_NEED_MORE);    /* This frame cannot be decoded! */
  }
  for(i = 0; i < main_data_begin; i++) {  /* Copy data from previous frames */
    id->g_main_data_vec[i] = id->g_main_data_vec[id->g_main_data_top - main_data_begin + i];
  }
  /* Read the main_data from file */
 (void) Get_Bytes(id,main_data_size,&(id->g_main_data_vec[main_data_begin]));
  /* Set up pointers */
  id->g_main_data_ptr = &(id->g_main_data_vec[0]);
  id->g_main_data_idx = 0;
  id->g_main_data_top = main_data_begin + main_data_size;
  return(PDMP3_OK);  /* Done */
}

/**Description: Reads audio and main data from bitstream into a buffer. main
*  data is taken from this frame and up to 2 previous frames.
* Parameters: Stream handle.
* Return value: PDMP3_OK or PDMP3_ERR if data could not be read,or contains errors.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Read_Audio_L3(pdmp3_handle *id){
  unsigned framesize,sideinfo_size,main_data_size,nch,ch,gr,scfsi_band,region,window;

  /* Number of channels(1 for mono and 2 for stereo) */
  nch =(id->g_frame_header.mode == mpeg1_mode_single_channel ? 1 : 2);
  /* Calculate header audio data size */
  framesize = (144 *
    g_mpeg1_bitrates[id->g_frame_header.layer-1][id->g_frame_header.bitrate_index]) /
    g_sampling_frequency[id->g_frame_header.sampling_frequency] +
    id->g_frame_header.padding_bit;
  if(framesize > 2000) {
    ERR("framesize = %d\n",framesize);
    return(PDMP3_ERR);
  }
  /* Sideinfo is 17 bytes for one channel and 32 bytes for two */
  sideinfo_size =(nch == 1 ? 17 : 32);
  /* Main data size is the rest of the frame,including ancillary data */
  main_data_size = framesize - sideinfo_size - 4 /* sync+header */;
  /* CRC is 2 bytes */
  if(id->g_frame_header.protection_bit == 0) main_data_size -= 2;
  /* DBG("framesize      =   %d\n",framesize); */
  /* DBG("sideinfo_size  =   %d\n",sideinfo_size); */
  /* DBG("main_data_size =   %d\n",main_data_size); */
  /* Read sideinfo from bitstream into buffer used by Get_Side_Bits() */
  Get_Sideinfo(id,sideinfo_size);
  if(Get_Filepos(id) == C_EOF) return(PDMP3_ERR);
  /* Parse audio data */
  /* Pointer to where we should start reading main data */
  id->g_side_info.main_data_begin = Get_Side_Bits(id,9);
  /* Get private bits. Not used for anything. */
  if(id->g_frame_header.mode == mpeg1_mode_single_channel)
    id->g_side_info.private_bits = Get_Side_Bits(id,5);
  else id->g_side_info.private_bits = Get_Side_Bits(id,3);
  /* Get scale factor selection information */
  for(ch = 0; ch < nch; ch++)
    for(scfsi_band = 0; scfsi_band < 4; scfsi_band++)
      id->g_side_info.scfsi[ch][scfsi_band] = Get_Side_Bits(id,1);
  /* Get the rest of the side information */
  for(gr = 0; gr < 2; gr++) {
    for(ch = 0; ch < nch; ch++) {
      id->g_side_info.part2_3_length[gr][ch]    = Get_Side_Bits(id,12);
      id->g_side_info.big_values[gr][ch]        = Get_Side_Bits(id,9);
      id->g_side_info.global_gain[gr][ch]       = Get_Side_Bits(id,8);
      id->g_side_info.scalefac_compress[gr][ch] = Get_Side_Bits(id,4);
      id->g_side_info.win_switch_flag[gr][ch]   = Get_Side_Bits(id,1);
      if(id->g_side_info.win_switch_flag[gr][ch] == 1) {
        id->g_side_info.block_type[gr][ch]       = Get_Side_Bits(id,2);
        id->g_side_info.mixed_block_flag[gr][ch] = Get_Side_Bits(id,1);
        for(region = 0; region < 2; region++)
          id->g_side_info.table_select[gr][ch][region] = Get_Side_Bits(id,5);
        for(window = 0; window < 3; window++)
          id->g_side_info.subblock_gain[gr][ch][window] = Get_Side_Bits(id,3);
        if((id->g_side_info.block_type[gr][ch]==2)&&(id->g_side_info.mixed_block_flag[gr][ch]==0))
          id->g_side_info.region0_count[gr][ch] = 8; /* Implicit */
        else id->g_side_info.region0_count[gr][ch] = 7; /* Implicit */
        /* The standard is wrong on this!!! */   /* Implicit */
        id->g_side_info.region1_count[gr][ch] = 20 - id->g_side_info.region0_count[gr][ch];
     }else{
       for(region = 0; region < 3; region++)
         id->g_side_info.table_select[gr][ch][region] = Get_Side_Bits(id,5);
       id->g_side_info.region0_count[gr][ch] = Get_Side_Bits(id,4);
       id->g_side_info.region1_count[gr][ch] = Get_Side_Bits(id,3);
       id->g_side_info.block_type[gr][ch] = 0;  /* Implicit */
      }  /* end if ... */
      id->g_side_info.preflag[gr][ch]            = Get_Side_Bits(id,1);
      id->g_side_info.scalefac_scale[gr][ch]     = Get_Side_Bits(id,1);
      id->g_side_info.count1table_select[gr][ch] = Get_Side_Bits(id,1);
    } /* end for(channel... */
  } /* end for(granule... */
  return(PDMP3_OK);/* Done */

}

/**Description: Reads 16 CRC bits
* Parameters: Stream handle.
* Return value: PDMP3_OK or PDMP3_ERR if CRC could not be read.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Read_CRC(pdmp3_handle *id){
  /* Get next two bytes from bitstream, If we got an End Of File we're done */
  if((Get_Byte(id)==C_EOF)||(Get_Byte(id)==C_EOF)) return(FALSE);
  return(PDMP3_OK);  /* Done */
}

/**Description: Search for next frame and read it into  buffer. Main data in
   this frame is saved for two frames since it might be needed by them.
* Parameters: Stream handle.
* Return value: PDMP3_OK if a frame is successfully read,PDMP3_ERR otherwise.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Read_Frame(pdmp3_handle *id){
  /* Try to find the next frame in the bitstream and decode it */
  if(Search_Header(id) != PDMP3_OK) return(PDMP3_ERR);
#ifdef DEBUG
  { static int framenum = 0;
    printf("\nFrame %d\n",framenum++);
    dmp_fr(&id->g_frame_header);
  }
    DBG("Starting decode,Layer: %d,Rate: %6d,Sfreq: %05d",
         id->g_frame_header.layer,
         g_mpeg1_bitrates[id->g_frame_header.layer - 1][id->g_frame_header.bitrate_index],
         g_sampling_frequency[id->g_frame_header.sampling_frequency]);
#endif
  /* Get CRC word if present */
  if((id->g_frame_header.protection_bit==0)&&(Read_CRC(id)!=PDMP3_OK)) return(PDMP3_ERR);
  if(id->g_frame_header.layer == 3) {  /* Get audio data */
    Read_Audio_L3(id);  /* Get side info */
    dmp_si(&id->g_frame_header,&id->g_side_info); /* DEBUG */
    /* If there's not enough main data in the bit reservoir,
     * signal to calling function so that decoding isn't done! */
    /* Get main data(scalefactors and Huffman coded frequency data) */
    return(Read_Main_L3(id));
  }else{
    ERR("Only layer 3(!= %d) is supported!\n",id->g_frame_header.layer);
    return(PDMP3_ERR);
  }
  return(PDMP3_OK);
}

/**Description: Scans bitstream for syncword until we find it or EOF. The
   syncword must be byte-aligned. It then reads and parses audio header.
* Parameters: Stream handle.
* Return value: PDMP3_OK or PDMP3_ERR if the syncword can't be found,or the header
*               contains impossible values.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Read_Header(pdmp3_handle *id) {
  unsigned b1,b2,b3,b4,header;
  /* Get the next four bytes from the bitstream */
  b1 = Get_Byte(id);
  b2 = Get_Byte(id);
  b3 = Get_Byte(id);
  b4 = Get_Byte(id);
  /* If we got an End Of File condition we're done */
  if((b1==C_EOF)||(b2==C_EOF)||(b3==C_EOF)||(b4==C_EOF))
    return(PDMP3_ERR);
  header =(b1 << 24) |(b2 << 16) |(b3 << 8) |(b4 << 0);
  /* Are the high 12 bits the syncword(0xfff)? */
  while((header & 0xfff00000) != C_SYNC) {
    /* No,so scan the bitstream one byte at a time until we find it or EOF */
    /* Shift the values one byte to the left */
    b1 = b2;
    b2 = b3;
    b3 = b4;
    /* Get one new byte from the bitstream */
    b4 = Get_Byte(id);
    /* If we got an End Of File condition we're done */
    if(b4 == C_EOF) return(PDMP3_ERR);
    /* Make up the new header */
    header =(b1 << 24) |(b2 << 16) |(b3 << 8) |(b4 << 0);
  } /* while... */
  /* If we get here we've found the sync word,and can decode the header
   * which is in the low 20 bits of the 32-bit sync+header word. */
  /* Decode the header */
  id->g_frame_header.id                 =(header & 0x00080000) >> 19;
  id->g_frame_header.layer              =(header & 0x00060000) >> 17;
  id->g_frame_header.protection_bit     =(header & 0x00010000) >> 16;
  id->g_frame_header.bitrate_index      =(header & 0x0000f000) >> 12;
  id->g_frame_header.sampling_frequency =(header & 0x00000c00) >> 10;
  id->g_frame_header.padding_bit        =(header & 0x00000200) >> 9;
  id->g_frame_header.private_bit        =(header & 0x00000100) >> 8;
  id->g_frame_header.mode               =(header & 0x000000c0) >> 6;
  id->g_frame_header.mode_extension     =(header & 0x00000030) >> 4;
  id->g_frame_header.copyright          =(header & 0x00000008) >> 3;
  id->g_frame_header.original_or_copy   =(header & 0x00000004) >> 2;
  id->g_frame_header.emphasis           =(header & 0x00000003) >> 0;
  /* Check for invalid values and impossible combinations */
  if(id->g_frame_header.id != 1) {
    ERR("ID must be 1\nHeader word is 0x%08x at file pos %d\n",header,Get_Filepos(id));
    return(PDMP3_ERR);
  }
  if(id->g_frame_header.bitrate_index == 0) {
    ERR("Free bitrate format NIY!\nHeader word is 0x%08x at file pos %d\n",header,Get_Filepos(id));
    return(PDMP3_ERR); // exit(1);
  }
  if(id->g_frame_header.bitrate_index == 15) {
    ERR("bitrate_index = 15 is invalid!\nHeader word is 0x%08x at file pos %d\n",header,Get_Filepos(id));
    return(PDMP3_ERR);
  }
  if(id->g_frame_header.sampling_frequency == 3) {
    ERR("sampling_frequency = 3 is invalid!\n");
    ERR("Header word is 0x%08x at file pos %d\n",header,Get_Filepos(id));
    return(PDMP3_ERR);
  }
  if(id->g_frame_header.layer == 0) {
    ERR("layer = 0 is invalid!\n");
    ERR("Header word is 0x%08x at file pos %d\n",header,
   Get_Filepos(id));
    return(PDMP3_ERR);
  }
  id->g_frame_header.layer = 4 - id->g_frame_header.layer;
  /* DBG("Header         =   0x%08x\n",header); */
  if(!id->new_header) id->new_header = 1;
  return(PDMP3_OK);  /* Done */
}

static int Search_Header(pdmp3_handle *id) {
  unsigned pos = id->processed;
  unsigned mark = id->istart;
  int res = PDMP3_NEED_MORE;
  int cnt = 0;
  while(Get_Inbuf_Filled(id) > 4) {
    res = Read_Header(id);
    if (id->g_frame_header.layer == 3) {
      if(res == PDMP3_OK || res == PDMP3_NEW_FORMAT) break;
    }
    if (++mark == INBUF_SIZE) {
      mark = 0;
    }
    id->istart = mark;
    id->processed = pos;
    if (++cnt > (2*576)) return(PDMP3_ERR); /* more than one frame and still no header */
  }
  return res;
}

/**Description: reads main data for layer 3 from main_data bit reservoir.
* Parameters: Stream handle.
* Return value: PDMP3_OK or PDMP3_ERR if the data contains errors.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Read_Main_L3(pdmp3_handle *id){
  unsigned framesize,sideinfo_size,main_data_size,gr,ch,nch,sfb,win,slen1,slen2,nbits,part_2_start;
  int res;

  /* Number of channels(1 for mono and 2 for stereo) */
  nch =(id->g_frame_header.mode == mpeg1_mode_single_channel ? 1 : 2);

  /* Calculate header audio data size */
  framesize = (144 *
    g_mpeg1_bitrates[id->g_frame_header.layer-1][id->g_frame_header.bitrate_index]) /
    g_sampling_frequency[id->g_frame_header.sampling_frequency] +
    id->g_frame_header.padding_bit;

  if(framesize > 2000) {
    ERR("framesize = %d\n",framesize);
    return(PDMP3_ERR);
  }
  /* Sideinfo is 17 bytes for one channel and 32 bytes for two */
  sideinfo_size =(nch == 1 ? 17 : 32);
  /* Main data size is the rest of the frame,including ancillary data */
  main_data_size = framesize - sideinfo_size - 4 /* sync+header */;
  /* CRC is 2 bytes */
  if(id->g_frame_header.protection_bit == 0) main_data_size -= 2;
  /* Assemble main data buffer with data from this frame and the previous
   * two frames. main_data_begin indicates how many bytes from previous
   * frames that should be used. This buffer is later accessed by the
   * Get_Main_Bits function in the same way as the side info is.
   */
  res = Get_Main_Data(id,main_data_size,id->g_side_info.main_data_begin);
  if(res != PDMP3_OK) return(res); /* This could be due to not enough data in reservoir */
  for(gr = 0; gr < 2; gr++) {
    for(ch = 0; ch < nch; ch++) {
      part_2_start = Get_Main_Pos(id);
      /* Number of bits in the bitstream for the bands */
      slen1 = mpeg1_scalefac_sizes[id->g_side_info.scalefac_compress[gr][ch]][0];
      slen2 = mpeg1_scalefac_sizes[id->g_side_info.scalefac_compress[gr][ch]][1];
      if((id->g_side_info.win_switch_flag[gr][ch] != 0)&&(id->g_side_info.block_type[gr][ch] == 2)) {
        if(id->g_side_info.mixed_block_flag[gr][ch] != 0) {
          for(sfb = 0; sfb < 8; sfb++)
            id->g_main_data.scalefac_l[gr][ch][sfb] = Get_Main_Bits(id,slen1);
          for(sfb = 3; sfb < 12; sfb++) {
            nbits = (sfb < 6)?slen1:slen2;/*slen1 for band 3-5,slen2 for 6-11*/
            for(win = 0; win < 3; win++)
              id->g_main_data.scalefac_s[gr][ch][sfb][win]=Get_Main_Bits(id,nbits);
          }
        }else{
          for(sfb = 0; sfb < 12; sfb++){
            nbits = (sfb < 6)?slen1:slen2;/*slen1 for band 3-5,slen2 for 6-11*/
            for(win = 0; win < 3; win++)
              id->g_main_data.scalefac_s[gr][ch][sfb][win]=Get_Main_Bits(id,nbits);
          }
        }
      }else{ /* block_type == 0 if winswitch == 0 */
        /* Scale factor bands 0-5 */
        if((id->g_side_info.scfsi[ch][0] == 0) ||(gr == 0)) {
          for(sfb = 0; sfb < 6; sfb++)
            id->g_main_data.scalefac_l[gr][ch][sfb] = Get_Main_Bits(id,slen1);
        }else if((id->g_side_info.scfsi[ch][0] == 1) &&(gr == 1)) {
          /* Copy scalefactors from granule 0 to granule 1 */
          for(sfb = 0; sfb < 6; sfb++)
            id->g_main_data.scalefac_l[1][ch][sfb]=id->g_main_data.scalefac_l[0][ch][sfb];
        }
        /* Scale factor bands 6-10 */
        if((id->g_side_info.scfsi[ch][1] == 0) ||(gr == 0)) {
          for(sfb = 6; sfb < 11; sfb++)
            id->g_main_data.scalefac_l[gr][ch][sfb] = Get_Main_Bits(id,slen1);
        }else if((id->g_side_info.scfsi[ch][1] == 1) &&(gr == 1)) {
          /* Copy scalefactors from granule 0 to granule 1 */
          for(sfb = 6; sfb < 11; sfb++)
            id->g_main_data.scalefac_l[1][ch][sfb]=id->g_main_data.scalefac_l[0][ch][sfb];
        }
        /* Scale factor bands 11-15 */
        if((id->g_side_info.scfsi[ch][2] == 0) ||(gr == 0)) {
          for(sfb = 11; sfb < 16; sfb++)
            id->g_main_data.scalefac_l[gr][ch][sfb] = Get_Main_Bits(id,slen2);
        } else if((id->g_side_info.scfsi[ch][2] == 1) &&(gr == 1)) {
          /* Copy scalefactors from granule 0 to granule 1 */
          for(sfb = 11; sfb < 16; sfb++)
            id->g_main_data.scalefac_l[1][ch][sfb]=id->g_main_data.scalefac_l[0][ch][sfb];
        }
        /* Scale factor bands 16-20 */
        if((id->g_side_info.scfsi[ch][3] == 0) ||(gr == 0)) {
          for(sfb = 16; sfb < 21; sfb++)
            id->g_main_data.scalefac_l[gr][ch][sfb] = Get_Main_Bits(id,slen2);
        }else if((id->g_side_info.scfsi[ch][3] == 1) &&(gr == 1)) {
          /* Copy scalefactors from granule 0 to granule 1 */
          for(sfb = 16; sfb < 21; sfb++)
            id->g_main_data.scalefac_l[1][ch][sfb]=id->g_main_data.scalefac_l[0][ch][sfb];
        }
      }
      /* Read Huffman coded data. Skip stuffing bits. */
      Read_Huffman(id,part_2_start,gr,ch);
    } /* end for(gr... */
  } /* end for(ch... */
  /* The ancillary data is stored here,but we ignore it. */
  return(PDMP3_OK);  /* Done */
}

/**Description: sets position of next bit to be read from main data bitstream.
* Parameters: Stream handle,Bit position. 0 = start,8 = start of byte 1,etc.
* Return value: PDMP3_OK or PDMP3_ERR if bit_pos is past end of main data for this frame.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Set_Main_Pos(pdmp3_handle *id,unsigned bit_pos){

  id->g_main_data_ptr = &(id->g_main_data_vec[bit_pos >> 3]);
  id->g_main_data_idx = bit_pos & 0x7;

  return(PDMP3_OK);

}

/**Description: returns next byte from bitstream, or EOF.
*  If we're not on an byte-boundary, bits remaining until next boundary are
*  discarded before getting that byte.
* Parameters: Stream handle.
* Return value: The next byte in bitstream in the lowest 8 bits,or C_EOF.
* Original Author: Krister Lagerström(krister@kmlager.com)
* Author: Erik Hofman(erik@ehofman.com) **/
static unsigned Get_Byte(pdmp3_handle *id){
  unsigned val = C_EOF;
  if(id->istart != id->iend){
    val = id->in[id->istart++]; //  && 0xff;
    if(id->istart == INBUF_SIZE){
      id->istart=0;
    }
    id->processed++;
  }
  return(val);
}

/**Description: returns current file position in bytes.
* Parameters: Stream handle.
* Return value: File pos in bytes,or 0 if no file open.
* Original Author: Krister Lagerström(krister@kmlager.com)
* Author: Erik Hofman(erik@ehofman.com) **/
static unsigned Get_Filepos(pdmp3_handle *id){
  return(id->processed);
}

/**Description: gets one bit from the local buffer which contains main_data.
* Parameters: Stream handle.
* Return value: The bit is returned in the LSB of the return value.
* Author: Krister Lagerström(krister@kmlager.com) **/
static unsigned Get_Main_Bit(pdmp3_handle *id){
  unsigned tmp;

  tmp = id->g_main_data_ptr[0] >>(7 - id->g_main_data_idx);
  tmp &= 0x01;
  id->g_main_data_ptr +=(id->g_main_data_idx + 1) >> 3;
  id->g_main_data_idx =(id->g_main_data_idx + 1) & 0x07;
  return(tmp);  /* Done */
}

/**Description: reads 'number_of_bits' from local buffer containing main_data.
* Parameters: Stream handle,number_of_bits to read(max 24)
* Return value: The bits are returned in the LSB of the return value.
*
******************************************************************************/
static unsigned Get_Main_Bits(pdmp3_handle *id,unsigned number_of_bits){
  unsigned tmp;


  if(number_of_bits == 0) return(0);

  /* Form a word of the next four bytes */
  tmp =(id->g_main_data_ptr[0] << 24) |(id->g_main_data_ptr[1] << 16) |
       (id->g_main_data_ptr[2] <<  8) |(id->g_main_data_ptr[3] <<  0);

  /* Remove bits already used */
  tmp = tmp << id->g_main_data_idx;

  /* Remove bits after the desired bits */
  tmp = tmp >>(32 - number_of_bits);

  /* Update pointers */
  id->g_main_data_ptr +=(id->g_main_data_idx + number_of_bits) >> 3;
  id->g_main_data_idx =(id->g_main_data_idx + number_of_bits) & 0x07;

  /* Done */
  return(tmp);

}

/**Description: returns pos. of next bit to be read from main data bitstream.
* Parameters: Stream handle.
* Return value: Bit position.
* Author: Krister Lagerström(krister@kmlager.com) **/
static unsigned Get_Main_Pos(pdmp3_handle *id){
  unsigned pos;
  
  pos =((size_t) id->g_main_data_ptr) -((size_t) &(id->g_main_data_vec[0]));
  pos /= 4; /* Divide by four to get number of bytes */
  pos *= 8;    /* Multiply by 8 to get number of bits */
  pos += id->g_main_data_idx;  /* Add current bit index */
  return(pos);
}

/**Description: reads 'number_of_bits' from buffer which contains side_info.
* Parameters: Stream handle,number_of_bits to read(max 16)
* Return value: The bits are returned in the LSB of the return value.
* Author: Krister Lagerström(krister@kmlager.com) **/
static unsigned Get_Side_Bits(pdmp3_handle *id,unsigned number_of_bits){
  unsigned tmp;

  /* Form a word of the next four bytes */                   //TODO endianness?
  tmp =(id->side_info_ptr[0] << 24) |(id->side_info_ptr[1] << 16) |
       (id->side_info_ptr[2] <<  8) |(id->side_info_ptr[3] <<  0);
  /* Remove bits already used */
  tmp = tmp << id->side_info_idx;
  /* Remove bits after the desired bits */
  tmp = tmp >>(32 - number_of_bits);
  /* Update pointers */
  id->side_info_ptr +=(id->side_info_idx + number_of_bits) >> 3;
  id->side_info_idx =(id->side_info_idx + number_of_bits) & 0x07;
  return(tmp);
}

/**Description: TBD
* Parameters: TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void Error(const char *s,int e){
  (void) fwrite(s,1,strlen(s),stderr);
  exit(e);
}

/**Description: Reads sideinfo from bitstream into buffer for Get_Side_Bits.
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void Get_Sideinfo(pdmp3_handle *id,unsigned sideinfo_size){
  if(Get_Bytes(id,sideinfo_size,id->side_info_vec) != PDMP3_OK) {
    ERR("\nCouldn't read sideinfo %d bytes at pos %d\n",
   sideinfo_size,Get_Filepos(id));
    return;
  }

  id->side_info_ptr = &(id->side_info_vec[0]);
  id->side_info_idx = 0;

}

/**Description: reads/decodes next Huffman code word from main_data reservoir.
* Parameters: Stream handle,Huffman table number and four pointers for the
              return values.
* Return value: Two(x,y) or four(x,y,v,w) decoded Huffman words.
* Author: Krister Lagerström(krister@kmlager.com) **/
static int Huffman_Decode(pdmp3_handle *id,unsigned table_num,int32_t *x,int32_t *y,int32_t *v,int32_t *w){
  unsigned point=0,error=1,bitsleft=32, //=16??
    treelen = g_huffman_main[table_num].treelen,
    linbits = g_huffman_main[table_num].linbits;

  treelen = g_huffman_main[table_num].treelen;
  if(treelen == 0) { /* Check for empty tables */
    *x = *y = *v = *w = 0;
    return(PDMP3_OK);
  }
  const unsigned short *htptr = g_huffman_main[table_num].hufftable;
  do {   /* Start reading the Huffman code word,bit by bit */
    /* Check if we've matched a code word */
    if((htptr[point] & 0xff00) == 0) {
      error = 0;
      *x =(htptr[point] >> 4) & 0xf;
      *y = htptr[point] & 0xf;
      break;
    }
    if(Get_Main_Bit(id)) { /* Go right in tree */
      while((htptr[point] & 0xff) >= 250)
        point += htptr[point] & 0xff;
      point += htptr[point] & 0xff;
    }else{ /* Go left in tree */
      while((htptr[point] >> 8) >= 250)
        point += htptr[point] >> 8;
      point += htptr[point] >> 8;
    }
  } while((--bitsleft > 0) &&(point < treelen));
  if(error) {  /* Check for error. */
    ERR("Illegal Huff code in data. bleft = %d,point = %d. tab = %d.",
      bitsleft,point,table_num);
    *x = *y = 0;
  }
  if(table_num > 31) {  /* Process sign encodings for quadruples tables. */
    *v =(*y >> 3) & 1;
    *w =(*y >> 2) & 1;
    *x =(*y >> 1) & 1;
    *y = *y & 1;
    if((*v > 0)&&(Get_Main_Bit(id) == 1)) *v = -*v;
    if((*w > 0)&&(Get_Main_Bit(id) == 1)) *w = -*w;
    if((*x > 0)&&(Get_Main_Bit(id) == 1)) *x = -*x;
    if((*y > 0)&&(Get_Main_Bit(id) == 1)) *y = -*y;
  }else{
    if((linbits > 0)&&(*x == 15))*x += Get_Main_Bits(id,linbits);/* Get linbits */
    if((*x > 0)&&(Get_Main_Bit(id) == 1)) *x = -*x; /* Get sign bit */
    if((linbits > 0)&&(*y == 15))*y += Get_Main_Bits(id,linbits);/* Get linbits */
    if((*y > 0)&&(Get_Main_Bit(id) == 1)) *y = -*y;/* Get sign bit */
  }
  return(error ? PDMP3_ERR : PDMP3_OK);  /* Done */
}

/**Description: Does inverse modified DCT and windowing.
* Parameters: TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void IMDCT_Win(float in[18],float out[36],unsigned block_type){
  unsigned i,m,N,p;
  float sum,tin[18];
#ifndef IMDCT_TABLES
  static float g_imdct_win[4][36];
  static unsigned init = 1;
//TODO : move to separate init function
  if(init) { /* Setup the four(one for each block type) window vectors */
    for(i = 0; i < 36; i++)  g_imdct_win[0][i] = sin(C_PI/36 *(i + 0.5)); //0
    for(i = 0; i < 18; i++)  g_imdct_win[1][i] = sin(C_PI/36 *(i + 0.5)); //1
    for(i = 18; i < 24; i++) g_imdct_win[1][i] = 1.0;
    for(i = 24; i < 30; i++) g_imdct_win[1][i] = sin(C_PI/12 *(i + 0.5 - 18.0));
    for(i = 30; i < 36; i++) g_imdct_win[1][i] = 0.0;
    for(i = 0; i < 12; i++)  g_imdct_win[2][i] = sin(C_PI/12 *(i + 0.5)); //2
    for(i = 12; i < 36; i++) g_imdct_win[2][i] = 0.0;
    for(i = 0; i < 6; i++)   g_imdct_win[3][i] = 0.0; //3
    for(i = 6; i < 12; i++)  g_imdct_win[3][i] = sin(C_PI/12 *(i + 0.5 - 6.0));
    for(i = 12; i < 18; i++) g_imdct_win[3][i] = 1.0;
    for(i = 18; i < 36; i++) g_imdct_win[3][i] = sin(C_PI/36 *(i + 0.5));
    init = 0;
  } /* end of init */
#endif
  for(i = 0; i < 36; i++) out[i] = 0.0;
  for(i = 0; i < 18; i++) tin[i] = in[i];
  if(block_type == 2) { /* 3 short blocks */
    N = 12;
    for(i = 0; i < 3; i++) {
      for(p = 0; p < N; p++) {
        sum = 0.0;
        for(m = 0;m < N/2; m++)
#ifdef IMDCT_NTABLES
          sum += tin[i+3*m] * cos_N12[m][p];
#else
          sum += tin[i+3*m] * cos(C_PI/(2*N)*(2*p+1+N/2)*(2*m+1));
#endif
        out[6*i+p+6] += sum * g_imdct_win[block_type][p]; //TODO FIXME +=?
      }
    } /* end for(i... */
  }else{ /* block_type != 2 */
    N = 36;
    for(p = 0; p < N; p++){
      sum = 0.0;
      for(m = 0; m < N/2; m++)
#ifdef IMDCT_NTABLES
        sum += in[m] * cos_N36[m][p];
#else
        sum += in[m] * cos(C_PI/(2*N)*(2*p+1+N/2)*(2*m+1));
#endif
      out[p] = sum * g_imdct_win[block_type][p];
    }
  }
}

/**Description: TBD
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void L3_Antialias(pdmp3_handle *id,unsigned gr,unsigned ch){
  unsigned sb /* subband of 18 samples */,i,sblim,ui,li;
  float ub,lb;

  /* No antialiasing is done for short blocks */
  if((id->g_side_info.win_switch_flag[gr][ch] == 1) &&
     (id->g_side_info.block_type[gr][ch] == 2) &&
     (id->g_side_info.mixed_block_flag[gr][ch]) == 0) {
    return; /* Done */
  }
  /* Setup the limit for how many subbands to transform */
  sblim =((id->g_side_info.win_switch_flag[gr][ch] == 1) &&
    (id->g_side_info.block_type[gr][ch] == 2) &&
    (id->g_side_info.mixed_block_flag[gr][ch] == 1))?2:32;
  /* Do the actual antialiasing */
  for(sb = 1; sb < sblim; sb++) {
    for(i = 0; i < 8; i++) {
      li = 18*sb-1-i;
      ui = 18*sb+i;
      lb = id->g_main_data.is[gr][ch][li]*cs[i] - id->g_main_data.is[gr][ch][ui]*ca[i];
      ub = id->g_main_data.is[gr][ch][ui]*cs[i] + id->g_main_data.is[gr][ch][li]*ca[i];
      id->g_main_data.is[gr][ch][li] = lb;
      id->g_main_data.is[gr][ch][ui] = ub;
    }
  }
  return; /* Done */
}

/**Description: TBD
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void L3_Frequency_Inversion(pdmp3_handle *id,unsigned gr,unsigned ch){
  unsigned sb,i;

  for(sb = 1; sb < 32; sb += 2) { //OPT? : for(sb = 18; sb < 576; sb += 36)
    for(i = 1; i < 18; i += 2)
      id->g_main_data.is[gr][ch][sb*18 + i] = -id->g_main_data.is[gr][ch][sb*18 + i];
  }
  return; /* Done */
}

/**Description: TBD
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void L3_Hybrid_Synthesis(pdmp3_handle *id,unsigned gr,unsigned ch){
  unsigned sb,i,j,bt;
  float rawout[36];
  static float store[2][32][18];

  if(id->hsynth_init) { /* Clear stored samples vector. OPT? use memset */
    for(j = 0; j < 2; j++) {
      for(sb = 0; sb < 32; sb++) {
        for(i = 0; i < 18; i++) {
          store[j][sb][i] = 0.0;
        }
      }
    }
    id->hsynth_init = 0;
  } /* end if(hsynth_init) */
  for(sb = 0; sb < 32; sb++) { /* Loop through all 32 subbands */
    /* Determine blocktype for this subband */
    bt =((id->g_side_info.win_switch_flag[gr][ch] == 1) &&
     (id->g_side_info.mixed_block_flag[gr][ch] == 1) &&(sb < 2))
      ? 0 : id->g_side_info.block_type[gr][ch];
    /* Do the inverse modified DCT and windowing */
    IMDCT_Win(&(id->g_main_data.is[gr][ch][sb*18]),rawout,bt);
    for(i = 0; i < 18; i++) { /* Overlapp add with stored vector into main_data vector */
      id->g_main_data.is[gr][ch][sb*18 + i] = rawout[i] + store[ch][sb][i];
      store[ch][sb][i] = rawout[i + 18];
    } /* end for(i... */
  } /* end for(sb... */
  return; /* Done */
}

/**Description: TBD
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void L3_Reorder(pdmp3_handle *id,unsigned gr,unsigned ch){
  unsigned sfreq,i,j,next_sfb,sfb,win_len,win;
  float re[576];

  sfreq = id->g_frame_header.sampling_frequency; /* Setup sampling freq index */
  /* Only reorder short blocks */
  if((id->g_side_info.win_switch_flag[gr][ch] == 1) &&
     (id->g_side_info.block_type[gr][ch] == 2)) { /* Short blocks */
    /* Check if the first two subbands
     *(=2*18 samples = 8 long or 3 short sfb's) uses long blocks */
    sfb = (id->g_side_info.mixed_block_flag[gr][ch] != 0)?3:0; /* 2 longbl. sb  first */
    next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
    win_len = g_sf_band_indices[sfreq].s[sfb+1] - g_sf_band_indices[sfreq].s[sfb];
    for(i =((sfb == 0) ? 0 : 36); i < 576; /* i++ done below! */) {
      /* Check if we're into the next scalefac band */
      if(i == next_sfb) {        /* Yes */
        /* Copy reordered data back to the original vector */
        for(j = 0; j < 3*win_len; j++)
          id->g_main_data.is[gr][ch][3*g_sf_band_indices[sfreq].s[sfb] + j] = re[j];
        /* Check if this band is above the rzero region,if so we're done */
        if(i >= id->g_side_info.count1[gr][ch]) return; /* Done */
        sfb++;
        next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
        win_len = g_sf_band_indices[sfreq].s[sfb+1] - g_sf_band_indices[sfreq].s[sfb];
      } /* end if(next_sfb) */
      for(win = 0; win < 3; win++) { /* Do the actual reordering */
        for(j = 0; j < win_len; j++) {
          re[j*3 + win] = id->g_main_data.is[gr][ch][i];
          i++;
        } /* end for(j... */
      } /* end for(win... */
    } /* end for(i... */
    /* Copy reordered data of last band back to original vector */
    for(j = 0; j < 3*win_len; j++)
      id->g_main_data.is[gr][ch][3 * g_sf_band_indices[sfreq].s[12] + j] = re[j];
  } /* end else(only long blocks) */
  return; /* Done */
}

/**Description: TBD
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void L3_Requantize(pdmp3_handle *id,unsigned gr,unsigned ch){
  unsigned sfb /* scalefac band index */,next_sfb /* frequency of next sfb */,
    sfreq,i,j,win,win_len;

  /* Setup sampling frequency index */
  sfreq = id->g_frame_header.sampling_frequency;
  /* Determine type of block to process */
  if((id->g_side_info.win_switch_flag[gr][ch] == 1) && (id->g_side_info.block_type[gr][ch] == 2)) { /* Short blocks */
    /* Check if the first two subbands
     *(=2*18 samples = 8 long or 3 short sfb's) uses long blocks */
    if(id->g_side_info.mixed_block_flag[gr][ch] != 0) { /* 2 longbl. sb  first */
      /* First process the 2 long block subbands at the start */
      sfb = 0;
      next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
      for(i = 0; i < 36; i++) {
        if(i == next_sfb) {
          sfb++;
          next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
        } /* end if */
        Requantize_Process_Long(id,gr,ch,i,sfb);
      }
      /* And next the remaining,non-zero,bands which uses short blocks */
      sfb = 3;
      next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
      win_len = g_sf_band_indices[sfreq].s[sfb+1] -
        g_sf_band_indices[sfreq].s[sfb];

      for(i = 36; i < id->g_side_info.count1[gr][ch]; /* i++ done below! */) {
        /* Check if we're into the next scalefac band */
        if(i == next_sfb) {        /* Yes */
          sfb++;
          next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
          win_len = g_sf_band_indices[sfreq].s[sfb+1] -
            g_sf_band_indices[sfreq].s[sfb];
        } /* end if(next_sfb) */
        for(win = 0; win < 3; win++) {
          for(j = 0; j < win_len; j++) {
            Requantize_Process_Short(id,gr,ch,i,sfb,win);
            i++;
          } /* end for(j... */
        } /* end for(win... */

      } /* end for(i... */
    }else{ /* Only short blocks */
      sfb = 0;
      next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
      win_len = g_sf_band_indices[sfreq].s[sfb+1] -
        g_sf_band_indices[sfreq].s[sfb];
      for(i = 0; i < id->g_side_info.count1[gr][ch]; /* i++ done below! */) {
        /* Check if we're into the next scalefac band */
        if(i == next_sfb) {        /* Yes */
          sfb++;
          next_sfb = g_sf_band_indices[sfreq].s[sfb+1] * 3;
          win_len = g_sf_band_indices[sfreq].s[sfb+1] -
            g_sf_band_indices[sfreq].s[sfb];
        } /* end if(next_sfb) */
        for(win = 0; win < 3; win++) {
          for(j = 0; j < win_len; j++) {
            Requantize_Process_Short(id,gr,ch,i,sfb,win);
            i++;
          } /* end for(j... */
        } /* end for(win... */
      } /* end for(i... */
    } /* end else(only short blocks) */
  }else{ /* Only long blocks */
    sfb = 0;
    next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
    for(i = 0; i < id->g_side_info.count1[gr][ch]; i++) {
      if(i == next_sfb) {
        sfb++;
        next_sfb = g_sf_band_indices[sfreq].l[sfb+1];
      } /* end if */
      Requantize_Process_Long(id,gr,ch,i,sfb);
    }
  } /* end else(only long blocks) */
  return; /* Done */
}

/**Description: TBD
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void L3_Stereo(pdmp3_handle *id,unsigned gr){
  unsigned max_pos,i,sfreq,sfb /* scalefac band index */;
  float left,right;

  /* Do nothing if joint stereo is not enabled */
  if((id->g_frame_header.mode != 1)||(id->g_frame_header.mode_extension == 0)) return;
  /* Do Middle/Side("normal") stereo processing */
  if(id->g_frame_header.mode_extension & 0x2) {
    /* Determine how many frequency lines to transform */
    max_pos = id->g_side_info.count1[gr][!!(id->g_side_info.count1[gr][0] > id->g_side_info.count1[gr][1])];
    /* Do the actual processing */
    for(i = 0; i < max_pos; i++) {
      left =(id->g_main_data.is[gr][0][i] + id->g_main_data.is[gr][1][i])
        *(C_INV_SQRT_2);
      right =(id->g_main_data.is[gr][0][i] - id->g_main_data.is[gr][1][i])
        *(C_INV_SQRT_2);
      id->g_main_data.is[gr][0][i] = left;
      id->g_main_data.is[gr][1][i] = right;
    } /* end for(i... */
  } /* end if(ms_stereo... */
  /* Do intensity stereo processing */
  if(id->g_frame_header.mode_extension & 0x1) {
    /* Setup sampling frequency index */
    sfreq = id->g_frame_header.sampling_frequency;
    /* First band that is intensity stereo encoded is first band scale factor
     * band on or above count1 frequency line. N.B.: Intensity stereo coding is
     * only done for higher subbands, but logic is here for lower subbands. */
    /* Determine type of block to process */
    if((id->g_side_info.win_switch_flag[gr][0] == 1) &&
       (id->g_side_info.block_type[gr][0] == 2)) { /* Short blocks */
      /* Check if the first two subbands
       *(=2*18 samples = 8 long or 3 short sfb's) uses long blocks */
      if(id->g_side_info.mixed_block_flag[gr][0] != 0) { /* 2 longbl. sb  first */
        for(sfb = 0; sfb < 8; sfb++) {/* First process 8 sfb's at start */
          /* Is this scale factor band above count1 for the right channel? */
          if(g_sf_band_indices[sfreq].l[sfb] >= id->g_side_info.count1[gr][1])
            Stereo_Process_Intensity_Long(id,gr,sfb);
        } /* end if(sfb... */
        /* And next the remaining bands which uses short blocks */
        for(sfb = 3; sfb < 12; sfb++) {
          /* Is this scale factor band above count1 for the right channel? */
          if(g_sf_band_indices[sfreq].s[sfb]*3 >= id->g_side_info.count1[gr][1])
            Stereo_Process_Intensity_Short(id,gr,sfb); /* intensity stereo processing */
        }
      }else{ /* Only short blocks */
        for(sfb = 0; sfb < 12; sfb++) {
          /* Is this scale factor band above count1 for the right channel? */
          if(g_sf_band_indices[sfreq].s[sfb]*3 >= id->g_side_info.count1[gr][1])
            Stereo_Process_Intensity_Short(id,gr,sfb); /* intensity stereo processing */
        }
      } /* end else(only short blocks) */
    }else{                        /* Only long blocks */
      for(sfb = 0; sfb < 21; sfb++) {
        /* Is this scale factor band above count1 for the right channel? */
        if(g_sf_band_indices[sfreq].l[sfb] >= id->g_side_info.count1[gr][1]) {
          /* Perform the intensity stereo processing */
          Stereo_Process_Intensity_Long(id,gr,sfb);
        }
      }
    } /* end else(only long blocks) */
  } /* end if(intensity_stereo processing) */
}

/**Description: TBD
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void L3_Subband_Synthesis(pdmp3_handle *id,unsigned gr,unsigned ch,unsigned outdata[576]){
  float u_vec[512],s_vec[32],sum; /* u_vec can be used insted of s_vec */
  int32_t samp;
  static unsigned init = 1;
  unsigned i,j,ss,nch;
  static float g_synth_n_win[64][32],v_vec[2 /* ch */][1024];

  /* Number of channels(1 for mono and 2 for stereo) */
  nch =(id->g_frame_header.mode == mpeg1_mode_single_channel) ? 1 : 2 ;
  /* Setup the n_win windowing vector and the v_vec intermediate vector */

  if(init) {
    for(i = 0; i < 64; i++) {
      for(j = 0; j < 32; j++) /*TODO: put in lookup table*/
        g_synth_n_win[i][j] = cos(((float)(16+i)*(2*j+1)) *(C_PI/64.0));
    }
    for(i = 0; i < 2; i++) /* Setup the v_vec intermediate vector */
      for(j = 0; j < 1024; j++) v_vec[i][j] = 0.0; /*TODO: memset */
    init = 0;
  } /* end if(init) */

  if(id->synth_init) {
    for(i = 0; i < 2; i++) /* Setup the v_vec intermediate vector */
      for(j = 0; j < 1024; j++) v_vec[i][j] = 0.0; /*TODO: memset*/
    id->synth_init = 0;
  } /* end if(synth_init) */

  for(ss = 0; ss < 18; ss++){ /* Loop through 18 samples in 32 subbands */
    for(i = 1023; i > 63; i--)  /* Shift up the V vector */
      v_vec[ch][i] = v_vec[ch][i-64];
    for(i = 0; i < 32; i++) /* Copy next 32 time samples to a temp vector */
      s_vec[i] =((float) id->g_main_data.is[gr][ch][i*18 + ss]);
    for(i = 0; i < 64; i++){ /* Matrix multiply input with n_win[][] matrix */
      sum = 0.0;
      for(j = 0; j < 32; j++) sum += g_synth_n_win[i][j] * s_vec[j];
      v_vec[ch][i] = sum;
    } /* end for(i... */
    for(i = 0; i < 8; i++) { /* Build the U vector */
      for(j = 0; j < 32; j++) { /* <<7 == *128 */
        u_vec[(i << 6) + j]      = v_vec[ch][(i << 7) + j];
        u_vec[(i << 6) + j + 32] = v_vec[ch][(i << 7) + j + 96];
      }
    } /* end for(i... */
    for(i = 0; i < 512; i++) /* Window by u_vec[i] with g_synth_dtbl[i] */
      u_vec[i] = u_vec[i] * g_synth_dtbl[i];
    for(i = 0; i < 32; i++) { /* Calc 32 samples,store in outdata vector */
      sum = 0.0;
      for(j = 0; j < 16; j++) /* sum += u_vec[j*32 + i]; */
        sum += u_vec[(j << 5) + i];
      /* sum now contains time sample 32*ss+i. Convert to 16-bit signed int */
      samp =(int32_t)(sum * 32767.0);
      if(samp > 32767) samp = 32767;
      else if(samp < -32767) samp = -32767;
      samp &= 0xffff;
      if(ch == 0) {  /* This function must be called for channel 0 first */
        /* We always run in stereo mode,& duplicate channels here for mono */
        if(nch == 1) {
          outdata[32*ss + i] =(samp << 16) |(samp);
        }else{
          outdata[32*ss + i] = samp << 16;
        }
      }else{
        outdata[32*ss + i] |= samp;
      }
    } /* end for(i... */
  } /* end for(ss... */
  return; /* Done */
}

/**Description: called by Read_Main_L3 to read Huffman coded data from bitstream.
* Parameters: Stream handle,TBD
* Return value: None. The data is stored in id->g_main_data.is[ch][gr][freqline].
* Author: Krister Lagerström(krister@kmlager.com) **/
static void Read_Huffman(pdmp3_handle *id,unsigned part_2_start,unsigned gr,unsigned ch){
  int32_t x,y,v,w;
  unsigned table_num,is_pos,bit_pos_end,sfreq;
  unsigned region_1_start,region_2_start; /* region_0_start = 0 */

  /* Check that there is any data to decode. If not,zero the array. */
  if(id->g_side_info.part2_3_length[gr][ch] == 0) {
    for(is_pos = 0; is_pos < 576; is_pos++)
      id->g_main_data.is[gr][ch][is_pos] = 0.0;
    return;
  }
  /* Calculate bit_pos_end which is the index of the last bit for this part. */
  bit_pos_end = part_2_start + id->g_side_info.part2_3_length[gr][ch] - 1;
  /* Determine region boundaries */
  if((id->g_side_info.win_switch_flag[gr][ch] == 1)&&
     (id->g_side_info.block_type[gr][ch] == 2)) {
    region_1_start = 36;  /* sfb[9/3]*3=36 */
    region_2_start = 576; /* No Region2 for short block case. */
  }else{
    sfreq = id->g_frame_header.sampling_frequency;
    region_1_start =
      g_sf_band_indices[sfreq].l[id->g_side_info.region0_count[gr][ch] + 1];
    region_2_start =
      g_sf_band_indices[sfreq].l[id->g_side_info.region0_count[gr][ch] +
        id->g_side_info.region1_count[gr][ch] + 2];
  }
  /* Read big_values using tables according to region_x_start */
  for(is_pos = 0; is_pos < id->g_side_info.big_values[gr][ch] * 2; is_pos++) {
    if(is_pos < region_1_start) {
      table_num = id->g_side_info.table_select[gr][ch][0];
    } else if(is_pos < region_2_start) {
      table_num = id->g_side_info.table_select[gr][ch][1];
    }else table_num = id->g_side_info.table_select[gr][ch][2];
    /* Get next Huffman coded words */
   (void) Huffman_Decode(id,table_num,&x,&y,&v,&w);
    /* In the big_values area there are two freq lines per Huffman word */
    id->g_main_data.is[gr][ch][is_pos++] = x;
    id->g_main_data.is[gr][ch][is_pos] = y;
  }
  /* Read small values until is_pos = 576 or we run out of huffman data */
  table_num = id->g_side_info.count1table_select[gr][ch] + 32;
  for(is_pos = id->g_side_info.big_values[gr][ch] * 2;
      (is_pos <= 572) &&(Get_Main_Pos(id) <= bit_pos_end); is_pos++) {
    /* Get next Huffman coded words */
   (void) Huffman_Decode(id,table_num,&x,&y,&v,&w);
    id->g_main_data.is[gr][ch][is_pos++] = v;
    if(is_pos >= 576) break;
    id->g_main_data.is[gr][ch][is_pos++] = w;
    if(is_pos >= 576) break;
    id->g_main_data.is[gr][ch][is_pos++] = x;
    if(is_pos >= 576) break;
    id->g_main_data.is[gr][ch][is_pos] = y;
  }
  /* Check that we didn't read past the end of this section */
  if(Get_Main_Pos(id) >(bit_pos_end+1)) /* Remove last words read */
    is_pos -= 4;
  /* Setup count1 which is the index of the first sample in the rzero reg. */
  id->g_side_info.count1[gr][ch] = is_pos;
  /* Zero out the last part if necessary */
  for(/* is_pos comes from last for-loop */; is_pos < 576; is_pos++)
    id->g_main_data.is[gr][ch][is_pos] = 0.0;
  /* Set the bitpos to point to the next part to read */
 (void) Set_Main_Pos(id,bit_pos_end+1);
  return;  /* Done */
}

/**Description: requantize sample in subband that uses long blocks.
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void Requantize_Process_Long(pdmp3_handle *id,unsigned gr,unsigned ch,unsigned is_pos,unsigned sfb){
  float tmp1,tmp2,tmp3,sf_mult,pf_x_pt;
  static float pretab[21] = { 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,3,3,3,2 };

  sf_mult = id->g_side_info.scalefac_scale[gr][ch] ? 1.0 : 0.5;
  pf_x_pt = id->g_side_info.preflag[gr][ch] * pretab[sfb];
  tmp1 = pow(2.0,-(sf_mult *(id->g_main_data.scalefac_l[gr][ch][sfb] + pf_x_pt)));
  tmp2 = pow(2.0,0.25 *((int32_t) id->g_side_info.global_gain[gr][ch] - 210));
  if(id->g_main_data.is[gr][ch][is_pos] < 0.0)
    tmp3 = -Requantize_Pow_43(-id->g_main_data.is[gr][ch][is_pos]);
  else tmp3 = Requantize_Pow_43(id->g_main_data.is[gr][ch][is_pos]);
  id->g_main_data.is[gr][ch][is_pos] = tmp1 * tmp2 * tmp3;
  return; /* Done */
}

/**Description: requantize sample in subband that uses short blocks.
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void Requantize_Process_Short(pdmp3_handle *id,unsigned gr,unsigned ch,unsigned is_pos,unsigned sfb,unsigned win){
  float tmp1,tmp2,tmp3,sf_mult;

  sf_mult = id->g_side_info.scalefac_scale[gr][ch] ? 1.0f : 0.5f;
  tmp1 = pow(2.0f,-(sf_mult * id->g_main_data.scalefac_s[gr][ch][sfb][win]));
  tmp2 = pow(2.0f,0.25f *((float) id->g_side_info.global_gain[gr][ch] - 210.0f -
              8.0f *(float) id->g_side_info.subblock_gain[gr][ch][win]));
  tmp3 =(id->g_main_data.is[gr][ch][is_pos] < 0.0)
    ? -Requantize_Pow_43(-id->g_main_data.is[gr][ch][is_pos])
    : Requantize_Pow_43(id->g_main_data.is[gr][ch][is_pos]);
  id->g_main_data.is[gr][ch][is_pos] = tmp1 * tmp2 * tmp3;
  return; /* Done */
}

/**Description: intensity stereo processing for entire subband with long blocks.
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void Stereo_Process_Intensity_Long(pdmp3_handle *id,unsigned gr,unsigned sfb){
  unsigned i,sfreq,sfb_start,sfb_stop,is_pos;
  float is_ratio_l,is_ratio_r,left,right;

  /* Check that((is_pos[sfb]=scalefac) != 7) => no intensity stereo */
  if((is_pos = id->g_main_data.scalefac_l[gr][0][sfb]) != 7) {
    sfreq = id->g_frame_header.sampling_frequency; /* Setup sampling freq index */
    sfb_start = g_sf_band_indices[sfreq].l[sfb];
    sfb_stop = g_sf_band_indices[sfreq].l[sfb+1];
    if(is_pos == 6) { /* tan((6*PI)/12 = PI/2) needs special treatment! */
      is_ratio_l = 1.0f;
      is_ratio_r = 0.0f;
    }else{
      is_ratio_l = is_ratios[is_pos] /(1.0f + is_ratios[is_pos]);
      is_ratio_r = 1.0f /(1.0f + is_ratios[is_pos]);
    }
    /* Now decode all samples in this scale factor band */
    for(i = sfb_start; i < sfb_stop; i++) {
      left = is_ratio_l * id->g_main_data.is[gr][0][i];
      right = is_ratio_r * id->g_main_data.is[gr][0][i];
      id->g_main_data.is[gr][0][i] = left;
      id->g_main_data.is[gr][1][i] = right;
    }
  }
  return; /* Done */
} /* end Stereo_Process_Intensity_Long() */

/**Description: This function is used to perform intensity stereo processing
*              for an entire subband that uses short blocks.
* Parameters: Stream handle,TBD
* Return value: TBD
* Author: Krister Lagerström(krister@kmlager.com) **/
static void Stereo_Process_Intensity_Short(pdmp3_handle *id,unsigned gr,unsigned sfb){
  unsigned sfb_start,sfb_stop,is_pos,is_ratio_l,is_ratio_r,i,sfreq,win,win_len;
  float left,right;

  sfreq = id->g_frame_header.sampling_frequency;   /* Setup sampling freq index */
  /* The window length */
  win_len = g_sf_band_indices[sfreq].s[sfb+1] - g_sf_band_indices[sfreq].s[sfb];
  /* The three windows within the band has different scalefactors */
  for(win = 0; win < 3; win++) {
    /* Check that((is_pos[sfb]=scalefac) != 7) => no intensity stereo */
    if((is_pos = id->g_main_data.scalefac_s[gr][0][sfb][win]) != 7) {
      sfb_start = g_sf_band_indices[sfreq].s[sfb]*3 + win_len*win;
      sfb_stop = sfb_start + win_len;
      if(is_pos == 6) { /* tan((6*PI)/12 = PI/2) needs special treatment! */
        is_ratio_l = 1.0;
        is_ratio_r = 0.0;
      }else{
        is_ratio_l = is_ratios[is_pos] /(1.0 + is_ratios[is_pos]);
        is_ratio_r = 1.0 /(1.0 + is_ratios[is_pos]);
      }
      /* Now decode all samples in this scale factor band */
      for(i = sfb_start; i < sfb_stop; i++) {
        left = is_ratio_l = id->g_main_data.is[gr][0][i];
        right = is_ratio_r = id->g_main_data.is[gr][0][i];
        id->g_main_data.is[gr][0][i] = left;
        id->g_main_data.is[gr][1][i] = right;
      }
    } /* end if(not illegal is_pos) */
  } /* end for(win... */
  return; /* Done */
} /* end Stereo_Process_Intensity_Short() */

#ifdef OUTPUT_RAW
/******************************************************************************
*
* Name: audio_write_raw
* Author: Krister Lagerström(krister@unidata.se)
* Description: This function is used to output raw data
* Parameters: Stream handle,file name,pointers to the samples,the number of
              samples
* Return value: None
* Revision History:
* Author   Date    Change
* krister  010101  Initial revision
*
******************************************************************************/
static void audio_write_raw(const char *filename,unsigned *samples,unsigned nbytes){
  static int init = 0,fd;
  char fname[1024];

  if(init == 0) {
    init = 1;
    if(strcmp(filename,"-")) {
      snprintf(fname,1023,"%s.raw",filename);
      fd = open(fname,O_WRONLY | O_CREAT,0666);
      if(fd == -1) {
        perror(fname);
        exit(-1);
      }
    } else {
      fd = 1;
    }
  }

  if(write(fd,samples,nbytes) != nbytes)
    Error("Unable to write raw data\n",-1);
  return;
} /* audio_write_raw() */
#endif

/**Description: output audio data
* Parameters: Stream handle,audio device name,file name.
* Return value: None
* Author: Krister Lagerström(krister@kmlager.com) **/
static void audio_write(pdmp3_handle *id,const char *audio_name,const char *filename,unsigned char *samples,size_t nbytes){
#ifdef OUTPUT_SOUND
  static int init = 0,audio,curr_sample_rate = 0;
  int format = AFMT_S16_LE,tmp,dsp_speed = 44100,dsp_stereo = 2;
  int sample_rate = g_sampling_frequency[id->g_frame_header.sampling_frequency];

  if(init == 0) {
    init = 1;
    audio = open(audio_name,O_WRONLY,0);
    if(audio == -1) {
      perror(audio_name);
      exit(-1);
    }
    tmp = format;
    ioctl(audio,SNDCTL_DSP_SETFMT,&format);
    if(tmp != format)
      Error("Unable to set the audio format\n",-1);
    if(ioctl(audio,SNDCTL_DSP_CHANNELS,&dsp_stereo) == -1)
      Error("Unable to set mono/stereo\n",-1);
  }

  if(curr_sample_rate != sample_rate) {
    curr_sample_rate = sample_rate;
    if(ioctl(audio,SNDCTL_DSP_SPEED,&dsp_speed) == -1)
      Error("Unable to set audio speed\n",-1);
  }

  if(write(audio,samples,nbytes) != nbytes)
    Error("Unable to write audio data\n",-1);
#endif /* OUTPUT_SOUND */
#ifdef OUTPUT_RAW
  audio_write_raw(filename,samples,nbytes);
#endif /* OUTPUT_RAW */
  return;
} /* audio_write() */


/*#############################################################################
 * Stream API - Added for AeonWave Audio (http://www.adalin.com)
 * This is a subset of the libmpg123 API and should by 100% compatible.
 *
 * Au0thor: Erik Hofman(erik@ehofman.com)
 */
static void Convert_Frame_S16(pdmp3_handle *id,unsigned char *outbuf,size_t buflen,size_t *done)
{
  short *s = (short *)outbuf;
  unsigned lo,hi,nsamps,framesz;
  int q,i,nch,gr;

  nch = (id->g_frame_header.mode == mpeg1_mode_single_channel ? 1 : 2);
  framesz = sizeof(short)*nch;

  nsamps = buflen / framesz;
  if (nsamps > (2*576 - id->ostart)) {
    nsamps = 2*576 - id->ostart;
  }
  *done = nsamps * framesz;

  /* copy to outbuf */
  i = id->ostart % 576;
  gr = id->ostart/576;
  for (q = 0; q < nsamps; ++q) {
    if(nch == 1) {
      lo = id->out[gr][i] & 0xffff;
      s[q] = lo;
    } else {
      lo = id->out[gr][i] & 0xffff;
      hi =(id->out[gr][i] & 0xffff0000) >> 16;
      s[2*q] = hi;
      s[2*q+1] = lo;
    }
    if (++i == 576) {
       ++gr;
       i = 0;
    }
  }

  id->ostart += nsamps;
  if (id->ostart == (2*576)) {
    id->ostart = 0;
  }
}

/**Description: Create a new streaming handle
* Parameters: None
* Return value: Stream handle
* Author: Erik Hofman(erik@ehofman.com) **/
pdmp3_handle* pdmp3_new(const char *decoder,int *error){
  return malloc(sizeof(pdmp3_handle));
}


/**Description: Free a streaming handle
* Parameters: Streaming handle
* Return value: None
* Author: Erik Hofman(erik@ehofman.com) **/
void pdmp3_delete(pdmp3_handle *id){
  free(id);
}


/**Description: Resets the stream handle.
* Parameters: Stream handle
* Return value: PDMP3_OK or PDMP3_ERR
* Author: Erik Hofman(erik@ehofman.com) **/
int pdmp3_open_feed(pdmp3_handle *id){
  if(id) {
    id->ostart = 0;
    id->istart = 0;
    id->iend = 0;
    id->processed = 0;
    id->new_header = 0;

    id->hsynth_init = 1;
    id->synth_init = 1;
    id->g_main_data_top = 0;

    return(PDMP3_OK);
  }
  return(PDMP3_ERR);
}

/**Description: Feed new data to the MP3 decoder
* Parameters: Streaming handle,data buffer containging MP3 data, size fo the
              data buffer.
* Return value: PDMP3_OK or an error
* Author: Erik Hofman(erik@ehofman.com) **/
int pdmp3_feed(pdmp3_handle *id,const unsigned char *in,size_t size){
  if(id && in && size) {
    int free = Get_Inbuf_Free(id);
    if(size<=free)
    {
      int res;
      if(id->iend<id->istart)
      {
         res = id->istart-id->iend;
         if(size<res) res=size;
         memcpy(id->in+id->iend,in,res);
         id->iend += res;
      }
      else
      {
         res = INBUF_SIZE-id->iend;
         if(size<res) res=size;
         if(res) {
            memcpy(id->in+id->iend,in,res);
            id->iend += res;
            size-= res;
         }
         if(size) {
            memcpy(id->in,in+res,size);
            id->iend = size;
         }
      }
      return(PDMP3_OK);
    }
    return(PDMP3_NO_SPACE);
  }
  return(PDMP3_ERR);
}

/**Description: Convert MP3 data to PCM data
* Parameters: Stream handle,a pointer to a buffer for the PCM data,the size of
              the PCM buffer in bytes,a pointer to return the number of
              converted bytes.
* Return value: PDMP3_OK or an error.
* Author: Erik Hofman(erik@ehofman.com) **/
int pdmp3_read(pdmp3_handle *id,unsigned char *outmemory,size_t outsize,size_t *done){
  if(id && outmemory && outsize && done) {
    *done = 0;
    if(outsize) {
      int res = PDMP3_ERR;

      if (id->ostart) {
        Convert_Frame_S16(id,outmemory,outsize,done);
        outmemory += *done;
        outsize -= *done;
        res = PDMP3_OK;
      }

      while(outsize) {
        if (Get_Inbuf_Filled(id) >= (2*576)) {
          size_t pos = id->processed;
          unsigned mark = id->istart;

          res = Read_Frame(id);
          if(res == PDMP3_OK || res == PDMP3_NEW_FORMAT) {
            size_t batch;

            Decode_L3(id);
            Convert_Frame_S16(id,outmemory,outsize,&batch);
            outmemory += batch;
            outsize -= batch;
            *done += batch;
          }
          else {
            id->processed = pos;
            id->istart = mark;
            break;
          }
        }
        else {
          res = PDMP3_NEED_MORE;
          break;
        }
      } /* outsize */
      if(id->new_header == 1 && res == PDMP3_OK) {
        res = PDMP3_NEW_FORMAT;
      }
      return(res);
    }
    else if(outsize < (2*576)) {
      return(PDMP3_NO_SPACE);
    }
    return(PDMP3_NEED_MORE);
  }
  return(PDMP3_ERR);
}

/**Description: Feed new data to the MP3 decoder and optionally convert it
                to PCM data.
* Parameters: Stream handle,a pinter to the MP3 data,size of the MP3 buffer,
              a pointer to a buffer for the PCM data or NULL,the size of
              the PCM buffer in bytes,a pointer to return the number of
  d->ostart            converted bytes.
* Return value: PDMP3_OK or an error.
* Author: Erik Hofman(erik@ehofman.com) **/
int pdmp3_decode(pdmp3_handle *id,const unsigned char *in,size_t insize,unsigned char *out,size_t outsize,size_t *done)
{
  int free = Get_Inbuf_Free(id);
  int res;

  *done = 0;
  if(free > insize) free = insize;
  res = pdmp3_feed(id,in,free);

  if(res == PDMP3_OK)
  {
    size_t avail;
    if(out && outsize) {
      res = pdmp3_read(id,out,outsize,&avail);
      *done = avail;
    }
    else if(Get_Filepos(id) == 0) {
      unsigned pos = id->processed;
      unsigned mark = id->istart;
      res = Search_Header(id);
      id->processed = pos;
      id->istart = mark;

      if(id->new_header == 1) {
          res = PDMP3_NEW_FORMAT;
      }
    }
  }
  return res;
}

/**Description: Get the current output format written to the addresses given.
* Parameters: Stream handle,pointers to store rate, channels and encoding.
* Return value: PDMP3_OK or an error
* Author: Erik Hofman(erik@ehofman.com) **/
int pdmp3_getformat(pdmp3_handle *id,long *rate,int *channels,int *encoding){
  if(id && rate && channels && encoding) {
    *encoding = PDMP3_ENC_SIGNED_16;
    *rate = g_sampling_frequency[id->g_frame_header.sampling_frequency];
    *channels = (id->g_frame_header.mode == mpeg1_mode_single_channel ? 1 : 2);
    id->new_header = -1;
    return(PDMP3_OK);
  }
  return(PDMP3_ERR);
}

/*#############################################################################
 * mp3s must be NULL terminated
 */
void pdmp3(char * const *mp3s){
  static const char *filename,*audio_name = "/dev/dsp";
  static FILE *fp =(FILE *) NULL;
  unsigned char out[INBUF_SIZE];
  pdmp3_handle *id;
  size_t done;
  int res;

  if(!strncmp("/dev/dsp",*mp3s,8)){
    audio_name = *mp3s++;
  }

  id = pdmp3_new(NULL,NULL);
  if(id == 0)
    Error("Cannot open stream API (out of memory)",0);

  while(*mp3s){
    filename = *mp3s++;
    if(!strcmp(filename,"-")) fp=stdin;
    else fp = fopen(filename,"r");
    if(fp == (FILE *) NULL)
      Error("Cannot open file\n",0);

    pdmp3_open_feed(id);
    while((res = pdmp3_read(id,out,INBUF_SIZE,&done)) != PDMP3_ERR){
      audio_write(id,audio_name,filename,out,done);
      if(res == PDMP3_OK || res == PDMP3_NEW_FORMAT) {
#ifdef DEBUG
        if(res == PDMP3_NEW_FORMAT) {
          int enc,channels;
          long rate;

          pdmp3_getformat(id,&rate,&channels,&enc);
          DBG("sample rate: %li Hz, no. channels: %i",rate,channels);
        }
#endif
      }
      else if(res == PDMP3_NEED_MORE){
        unsigned char in[4096];

        res = fread(in,1,4096,fp);
        if(!res) break;

        res = pdmp3_feed(id,in,res);
      }
    }
    fclose(fp);
  }
  pdmp3_delete(id);
}
#endif /* !definend(PDMP3_HEADER_ONLY) */
