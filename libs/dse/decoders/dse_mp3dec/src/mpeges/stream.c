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

#include <mpeges/stream.h>
#include <stdio.h>

void dse_mp3dec_bypass_id3v2(unsigned char** buf, const size_t buf_size) {

    unsigned char* current_buf = *buf;

    if (buf == NULL || *buf == NULL || buf_size <= 0) {
        return;
    }

    if (buf_size >= 10 && memcmp(current_buf, "ID3", 3) == 0) {

        unsigned long id3_len = ((current_buf[6] & 0x7F) << 21) |
            ((current_buf[7] & 0x7F) << 14) |
            ((current_buf[8] & 0x7F) << 7) |
            (current_buf[9] & 0x7F);


        size_t total_id3_len = 10 + id3_len;
        if (total_id3_len > buf_size) {
            return;
        }

        *buf += total_id3_len;
    }
}

int dse_mp3dec_read_header(
    MPEG_ES_HEADER* header,
    const unsigned char* in_buf,
    size_t in_buf_size
) {
    if (in_buf_size < sizeof(*header))
        return -1;

    if (strncmp((const char*)in_buf, "LAME3.", 6) == 0) {
        in_buf += 6;
    }

    if ((in_buf[0] != 0xFF) || !(in_buf[1] & 0xE0)) {
        return -2;
    }

    /* header->version - MPEG/MP3 version
     *
     * 0: ÌP3 Voice (aka. MPEG-2.5)
     * 1: Reversed
     * 2: ÌPEG-2
     * 3: ÌPEG-1
     */
    header->version         = (in_buf[1] & 0x18) >> 3;

    /* header->layer - MPEG/MP3 version
     *
     * 0: Reversed
     * 1: Layer III
     * 2: Layer II
     * 3: Layer I
     */
    header->layer           = (in_buf[1] & 0x06) >> 1;

    header->crc_b           = in_buf[1] & 0x01;
    header->br_idx          = (in_buf[2] & 0xF0) >> 4;
    header->sr_idx          = (in_buf[2] & 0x0C) >> 2;
    header->padding_b       = (in_buf[2] & 0x02) >> 1;
    header->private_b       = in_buf[2] & 0x01;
    header->mode            = (in_buf[3] & 0xC0) >> 6;
    header->mode_ext        = (in_buf[3] & 0x30) >> 4;
    header->copyright_b     = (in_buf[3] & 0x08) >> 3;
    header->original_b      = (in_buf[3] & 0x04) >> 2;
    header->emphasis        = in_buf[3] & 0x03;

    return 0;
}

void dse_mp3dec_sideinfo_decode(const unsigned char* data, MPEG_L3_SIDEINFO* si) {

    si->sfo = si->mdb + SIZE_OF_SIDE_INFO_BYTES;
    si->mdb = data[0] | (data[1] << 8);
    si->sfc = data[2];
    si->sfb = data[3];

    int offset = 4;
    for (int gr = 0; gr < 2; gr++) {
        for (int ch = 0; ch < 2; ch++) {
            for (int sfb = 0; sfb < 21; sfb++) {
                if (ch == 0) {
                    si->sfl[gr][sfb] = data[offset++];
                } else {
                    si->sfr[gr][sfb] = data[offset++];
                }
            }
        }
    }

    si->intens_stereo = (data[offset++] & 0x01) != 0;
    si->ms_stereo = (data[offset++] & 0x01) != 0;
}
