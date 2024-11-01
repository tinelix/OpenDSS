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

#ifndef DSE_DECODERS_PDMP3_LAYER3_H
#define DSE_DECODERS_PDMP3_LAYER3_H

typedef struct {  /* MPEG1 Layer 3 Side Information : [2][2] means [gr][ch] */
    unsigned main_data_begin;         /* 9 bits */
    unsigned private_bits;            /* 3 bits in mono,5 in stereo */
    unsigned scfsi[2][4];             /* 1 bit */
    unsigned part2_3_length[2][2];    /* 12 bits */
    unsigned big_values[2][2];        /* 9 bits */
    unsigned global_gain[2][2];       /* 8 bits */
    unsigned scalefac_compress[2][2]; /* 4 bits */
    unsigned win_switch_flag[2][2];   /* 1 bit */
    /* if(win_switch_flag[][]) */ //use a union dammit
    unsigned block_type[2][2];        /* 2 bits */
    unsigned mixed_block_flag[2][2];  /* 1 bit */
    unsigned table_select[2][2][3];   /* 5 bits */
    unsigned subblock_gain[2][2][3];  /* 3 bits */
    /* else */
    /* table_select[][][] */
    unsigned region0_count[2][2];     /* 4 bits */
    unsigned region1_count[2][2];     /* 3 bits */
    /* end */
    unsigned preflag[2][2];           /* 1 bit */
    unsigned scalefac_scale[2][2];    /* 1 bit */
    unsigned count1table_select[2][2];/* 1 bit */
    unsigned count1[2][2];            /* Not in file,calc. by huff.dec.! */
}
t_mpeg_side_info;

typedef struct { /* MPEG1 Layer 3 Main Data */
    unsigned  scalefac_l[2][2][21];    /* 0-4 bits */
    unsigned  scalefac_s[2][2][12][3]; /* 0-4 bits */
    float is[2][2][576];               /* Huffman coded freq. lines */
}
t_mpeg_main_data;

#endif // DSE_DECODERS_PDMP3_LAYER3_H
