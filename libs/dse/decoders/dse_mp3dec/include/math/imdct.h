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
#include <utils/intext.h>

double spectrum_values[SPECTRAL_LINES];

double time_domain_output[TIME_DOMAIN_POINTS];
void dse_mp3dec_imdct_overlap_add(
    double* prev_window, double* curr_window,
    double* output_signal, int overlap_size
);