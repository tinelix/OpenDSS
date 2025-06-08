#pragma once

#ifdef _WIN32
	#include <windows.h>
#endif

#include <stdio.h>

typedef struct {
	int bits_per_sample;
	int channels;
	int sample_rate;
} DSE_PCM_OUTPUT_FORMAT;

class SoundEngineWrapper {
public:
	SoundEngineWrapper();
	~SoundEngineWrapper();
	void init();
	void prepare();
	void openInputFile(const char fname[512]);
	void play();
	void closeInputFile();
private:
#ifdef _WIN32
	HMODULE _dse;
#endif
	void importLib();
};