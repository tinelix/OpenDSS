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

#ifndef DSE_DECODERS_PDMP3_SCALEFCT_H
#define DSE_DECODERS_PDMP3_SCALEFCT_H

/* Scale factor band indices
 *
 * One table per sample rate. Each table contains the frequency indices
 * for the 12 short and 21 long scalefactor bands. The short indices
 * must be multiplied by 3 to get the actual index.
 */
static const t_sf_band_indices g_sf_band_indices[3 /* Sampling freq. */] = {
    {
        {0,4,8,12,16,20,24,30,36,44,52,62,74,90,110,134,162,196,238,288,342,418,576},
        {0,4,8,12,16,22,30,40,52,66,84,106,136,192}
    },
    {
        {0,4,8,12,16,20,24,30,36,42,50,60,72,88,106,128,156,190,230,276,330,384,576},
        {0,4,8,12,16,22,28,38,50,64,80,100,126,192}
    },
    {
        {0,4,8,12,16,20,24,30,36,44,54,66,82,102,126,156,194,240,296,364,448,550,576},
        {0,4,8,12,16,22,30,42,58,78,104,138,180,192}
    }
};

#endif // DSE_DECODERS_PDMP3_SCALEFCT_H
