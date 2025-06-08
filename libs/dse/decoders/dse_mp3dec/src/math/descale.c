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

#include <math/descale.h>

void dse_mp3dec_descale(unsigned char* in_buf, MPEG_L3_FREQBAND* spectrum, const MPEG_L3_SIDEINFO* si) {
    float scale_factors[SFB_COUNT];

    dse_mp3dec_extract_sf(in_buf, si, scale_factors);

    for (int band = 0; band < SFB_COUNT; band++) {
        float scale_factor = scale_factors[band];

        for (unsigned char i = spectrum->start[band]; 
             i < spectrum->end[band]; i++
        ) {
            spectrum->values[i] *= scale_factor;
        }
    }
}

int dse_mp3dec_extract_sf(const unsigned char* data, const MPEG_L3_SIDEINFO* si, float* sf) {

    if (si->sfo > 1000000 || si->sfo < 0) {
        return -1;
    }

    size_t header_size = sizeof(MPEG_ES_HEADER);
    unsigned int sfo = si->sfo;
    size_t total_offset = header_size + sfo;
    const unsigned char* sf_ptr = data + total_offset;

    for (int band = 0; band < SFB_COUNT; band++) {
        sf[band] = pow(2.f, *(sf_ptr++));
    }

    return 0;
}