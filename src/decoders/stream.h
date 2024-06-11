/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU General Public License 3 (or any later version) and/or Apache License 2
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */

#pragma once

// Pulse Code Modulation (RAW)

#define STREAMINFO_CODEC_PCM_U8             0x2000      // or Unsigned PCM 8-bit
#define STREAMINFO_CODEC_PCM_S8             0x2001      // or Signed PCM 8-bit
#define STREAMINFO_CODEC_PCM_S16BE          0x2010      // or Signed PCM 16-bit Big-Endian
#define STREAMINFO_CODEC_PCM_S16LE          0x2011      // or Signed PCM 16-bit Little-Endian
#define STREAMINFO_CODEC_PCM_S24            0x2020
#define STREAMINFO_CODEC_PCM_S32            0x2030
#define STREAMINFO_CODEC_PCM_S32F           0x2031      // or Signed PCM 32-bit with floating point

// Free Lossless Audio Coding (*.flac)

#define STREAMINFO_CODEC_FLAC_8             0x3000      // or FLAC 8-bit
#define STREAMINFO_CODEC_FLAC_16            0x3001
#define STREAMINFO_CODEC_FLAC_24            0x3002

// MPEG Layer 3 (*.mp3)

#define STREAMINFO_CODEC_MPEG1_L3           0x4000
#define STREAMINFO_CODEC_MPEG2_L3           0x4001
#define STREAMINFO_CODEC_MPEG2_L3_EXT       0x4002      // or MPEG-2.5 Layer 3

// MPEG Advanced Audio Encoding (*.aac, *.m4a)

#define STREAMINFO_CODEC_AAC                0x4010
#define STREAMINFO_CODEC_HE_AAC             0x4011
#define STREAMINFO_CODEC_HE_AAC_V2          0x4012
#define STREAMINFO_CODEC_HE_AAC_V3          0x4013      // or xHE-AAC

struct StreamInfo {
    int codec, sampleRate, bitrate, channels, lengthSec;
};

struct StreamTimestamp {
    int position, duration;
};

struct AudioSpectrum {
    int left, right;
};

