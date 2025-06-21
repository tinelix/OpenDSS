#pragma once

#ifdef _WIN32
	#include <windows.h>
#endif

#include <interfaces/dsewrapi.h>

#include <stdio.h>

// Pulse Code Modulation (PCM)

#define STREAMINFO_CODEC_PCM_U8             0x2000      // or Unsigned PCM 8-bit
#define STREAMINFO_CODEC_PCM_S8             0x2001      // or Signed PCM 8-bit
#define STREAMINFO_CODEC_PCM_S16LE          0x2011      // or Signed PCM 16-bit little-endian
#define STREAMINFO_CODEC_PCM_S24LE          0x2021		// or Signed PCM 24-bit little-endian
#define STREAMINFO_CODEC_PCM_S32LE          0x2031		// or Signed PCM 32-bit little-endian
#define STREAMINFO_CODEC_PCM_S32LE_F        0x2032      // or Signed PCM 32-bit little-endian with floating point

// Free Lossless Audio Coding (*.flac)

#define STREAMINFO_CODEC_FLAC_8             0x3000      // or FLAC 8-bit
#define STREAMINFO_CODEC_FLAC_16            0x3001
#define STREAMINFO_CODEC_FLAC_24            0x3002

// MPEG Layer 3 (*.mp3)

#define STREAMINFO_CODEC_MPEG1_L3           0x4000
#define STREAMINFO_CODEC_MPEG2_L3           0x4001
#define STREAMINFO_CODEC_MPEG2_L3_EXT       0x4002      // or MPEG-2.5 Layer 3

// MPEG Advanced Audio Encoding (*.adts, *.aac, *.m4a)

#define STREAMINFO_CODEC_AAC                0x4010
#define STREAMINFO_CODEC_HE_AAC             0x4011
#define STREAMINFO_CODEC_HE_AAC_V2          0x4012
#define STREAMINFO_CODEC_HE_AAC_V3          0x4013      // or xHE-AAC

typedef struct {
	int channels;
	int bits_per_sample;
	int sample_rate;
} DSE_PCM_OUTPUT_FORMAT;

typedef struct {
	int codec_id;
	int sample_rate;
	int bits_per_sample;
	long bitrate;
	int channels;
	int bitrate_mode; // 0 - CBR, 1 - ABR, 2 - VBR
} DSE_STREAM_INFO;

class SoundEngineWrapper {
public:
	SoundEngineWrapper();
	~SoundEngineWrapper();
	void setInterface(ISoundEngineWrapper* pInterface);
	int init();
	void prepare();
	int  openInputFile(const char fname[512]);
	void play();
	DSE_STREAM_INFO getStreamInfo();
	void closeInputFile();
private:
#ifdef _WIN32
	HMODULE _dse;
	ISoundEngineWrapper* gInterface;
#endif
	void importLib();
};