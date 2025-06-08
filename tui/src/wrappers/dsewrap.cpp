#include "dsewrap.h"

#ifdef _WIN32
	typedef int (WINAPI* DSEInitFunction) ();
	typedef int (WINAPI* DSEPrepareFunction) (DSE_PCM_OUTPUT_FORMAT);
	typedef int (WINAPI* DSEOpenInputFunction) (const char[512]);
	typedef int (WINAPI* DSEDecodeFrameFunction)();
	typedef int (WINAPI* DSECloseInputFunction) ();
#endif

DSEInitFunction			dse_init;
DSEPrepareFunction		dse_prepare;
DSEOpenInputFunction	dse_open_input;
DSEDecodeFrameFunction	dse_decode_frame;
DSECloseInputFunction	dse_close_input;

int failed_attempts = 0;

SoundEngineWrapper::SoundEngineWrapper() {
	
}

SoundEngineWrapper::~SoundEngineWrapper() {
	#ifdef _WIN32
		FreeLibrary(_dse);
	#endif
}

void SoundEngineWrapper::init() {
	#ifdef _WIN32
		_dse = LoadLibrary("libs/opendse.dll");
	#endif

	if (!_dse) {
		errno = EINVAL;
		return;
	}
	importLib();
	dse_init();
}

void SoundEngineWrapper::prepare() {
	DSE_PCM_OUTPUT_FORMAT params;
	params.sample_rate = 44100;
	params.bits_per_sample = 16;
	params.channels = 2;

	dse_prepare(params);
}

void SoundEngineWrapper::openInputFile(const char fname[512]) {
	int result = dse_open_input(fname);
}

void SoundEngineWrapper::play() {

	while (failed_attempts < 4) {
		int result = dse_decode_frame();
		if (result < 0) {
			failed_attempts++;
		}
	}

	if (failed_attempts >= 4)
		closeInputFile();
}

void SoundEngineWrapper::closeInputFile() {
	int result = dse_close_input();
}

void SoundEngineWrapper::importLib() {

	#ifdef _WIN32
		dse_init = (DSEInitFunction)
			GetProcAddress(_dse, "dse_init");
		dse_prepare = (DSEPrepareFunction)
			GetProcAddress(_dse, "dse_prepare");
		dse_open_input = (DSEOpenInputFunction)
			GetProcAddress(_dse, "dse_open_input");
		dse_decode_frame = (DSEDecodeFrameFunction)
			GetProcAddress(_dse, "dse_decode_frame");
		dse_close_input = (DSECloseInputFunction)
			GetProcAddress(_dse, "dse_close_input");
	#endif

}

