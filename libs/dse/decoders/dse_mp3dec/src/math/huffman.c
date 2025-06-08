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

#include <math/huffman.h>

int dse_mp3dec_huffman_decode(
	const unsigned char* data,
	int* offset,
	const unsigned int* table
) {
    unsigned int bits = 0;
    int len = 0;

    if (!table || sizeof(*table) < 128)
        return -1;

    while (len < MAX_CODE_BITS) {
        bits <<= 1;
        bits |= (data[*offset++] & 0x1);
        len++;

        for (int i = 0; ; i += 2) {
            if ((table[i] & 0xFFFF) == len && (table[i + 1] & 0xFFFF) == bits) {
                return table[i + 1] >> 16;
            }
            if ((table[i] & 0xFFFF) == 0) break;
        }
    }

    return -2;
}

const unsigned int* dse_mp3dec_huffman_select(const MPEG_L3_SIDEINFO* si) {
    // Check the block type to determine which group of tables to use
    if ((si->blk_type & 0x80) != 0) {
        return MPEG_L3_HUFFMAN[REGION_SHORT];
    } else if ((si->blk_type & 0x40) != 0) {
        return MPEG_L3_HUFFMAN[REGION_LONG];
    } else {
        return NULL;
    }

    // Additional conditions can be added here based on stereo modes etc.
}