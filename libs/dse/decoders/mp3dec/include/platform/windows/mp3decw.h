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

#ifndef _MP3DEC_PLATFORM_WINDOWS_MAIN_H
#define _MP3DEC_PLATFORM_WINDOWS_MAIN_H

#define LIB_EXPORT __declspec(dllexport)
#define LIB_IMPORT __declspec(dllimport)

typedef struct {
	int sample_rate;
	int channels;
	int bits_per_sample;
} DSE_AUDIO_OUTPUT_INFO;

#endif
