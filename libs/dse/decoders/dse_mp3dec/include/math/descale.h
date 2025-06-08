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

#pragma once

#include <mpeges/stream.h>

void dse_mp3dec_descale(
	unsigned char* in_buf, MPEG_L3_FREQBAND* spectrum, const MPEG_L3_SIDEINFO* si
);
int dse_mp3dec_extract_sf(
	const unsigned char* data, const MPEG_L3_SIDEINFO* si, float* sf
);