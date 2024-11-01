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

#ifndef DSE_DECODERS_PDMP3_MPEGAUDIO_H
#define DSE_DECODERS_PDMP3_MPEGAUDIO_H

static const unsigned g_mpeg1_bitrates[3 /* layer 1-3 */][15 /* header bitrate_index */] = {
    {    /* Layer 1 */
        0,  32000, 64000, 96000,128000,160000,192000,224000,
           256000,288000,320000,352000,384000,416000,448000
    },{  /* Layer 2 */
        0,  32000, 48000, 56000, 64000, 80000, 96000,112000,
           128000,160000,192000,224000,256000,320000,384000
    },{   /* Layer 3 */
        0, 32000, 40000, 48000, 56000, 64000, 80000, 96000,
          112000,128000,160000,192000,224000,256000,320000
    }
},
g_sampling_frequency[3] = { 44100 * Hz, 48000 * Hz, 32000 * Hz },
mpeg1_scalefac_sizes[16][2 /* slen1,slen2 */] = {
    {0,0},{0,1},{0,2},{0,3},{3,0},{1,1},{1,2},{1,3},
    {2,1},{2,2},{2,3},{3,1},{3,2},{3,3},{4,2},{4,3}
};

#endif // DSE_DECODERS_PDMP3_MPEGAUDIO_H
