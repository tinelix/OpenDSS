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

#ifndef DSE_DECODERS_PDMP3_HEADER_H
#define DSE_DECODERS_PDMP3_HEADER_H

/* Layer number */
typedef enum {
    mpeg_layer_reserved = 0,
    mpeg_layer_3        = 1,
    mpeg_layer_2        = 2,
    mpeg_layer_1        = 3
}
t_mpeg_layer;

/* Modes */
typedef enum {
    mpeg_mode_stereo = 0,
    mpeg_mode_joint_stereo,
    mpeg_mode_dual_channel,
    mpeg_mode_single_channel
}
t_mpeg_mode;

/* MPEG Layer 1-3 frame header */
typedef struct {
    unsigned id;                 // 1 bit
    t_mpeg_layer layer;          // 2 bits
    unsigned protection_bit;     // 1 bit
    unsigned bitrate_index;      // 4 bits
    unsigned sampling_frequency; // 2 bits
    unsigned padding_bit;        // 1 bit
    unsigned private_bit;        // 1 bit
    t_mpeg_mode mode;            // 2 bits
    unsigned mode_extension;     // 2 bits
    unsigned copyright;          // 1 bit
    unsigned original_or_copy;   // 1 bit
    unsigned emphasis;           // 2 bits
}
t_mpeg_header;

#endif // DSE_DECODERS_PDMP3_HEADER_H
