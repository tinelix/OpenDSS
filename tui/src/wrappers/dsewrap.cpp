#include "dsewrap.h"

#ifdef _WIN32
	typedef int		(WINAPI* DSEInitFunction)		 ();
	typedef int		(WINAPI* DSEPrepareFunction)	 (DSE_PCM_OUTPUT_FORMAT);
	typedef int		(WINAPI* DSEOpenInputFunction)	 (const char[512]);
	typedef void	(WINAPI* DSEAllocateFrameFunction) ();
	typedef int		(WINAPI* DSEDecodeFrameFunction) ();
	typedef void	(WINAPI* DSEPlayFunction) ();
	typedef void	(WINAPI* DSEFreeFrameFunction) ();
	typedef double	(WINAPI* DSEGetFrameRMSFunction) ();
	typedef int		(WINAPI* DSEIsEndOfFileFunction) ();
	typedef int		(WINAPI* DSECloseInputFunction)  ();
#endif

DSEInitFunction				dse_init;
DSEPrepareFunction			dse_prepare;
DSEOpenInputFunction		dse_open_input;
DSEAllocateFrameFunction	dse_allocate_frame;
DSEDecodeFrameFunction		dse_decode_frame;
DSEPlayFunction				dse_play;
DSEFreeFrameFunction		dse_free_frame;
DSEGetFrameRMSFunction		dse_get_frame_rms;
DSEIsEndOfFileFunction      dse_feof;
DSECloseInputFunction		dse_close_input;

int failed_attempts = 0;

SoundEngineWrapper::SoundEngineWrapper() {
	
}

SoundEngineWrapper::~SoundEngineWrapper() {
	#ifdef _WIN32
		FreeLibrary(_dse);
	#endif
}

int SoundEngineWrapper::init() {
	#ifdef _WIN32
		_dse = LoadLibrary("libs/opendse.dll");
	#endif

	if (_dse == NULL) {
		return -4;
	}
	importLib();
	dse_init();
	return 0;
}

void SoundEngineWrapper::prepare() {
	DSE_PCM_OUTPUT_FORMAT params;
	params.sample_rate = 44100;
	params.bits_per_sample = 16;
	params.channels = 2;

	dse_prepare(params);
}

int SoundEngineWrapper::openInputFile(const char fname[512]) {
	int result = dse_open_input(fname);
	return result;
}

void SoundEngineWrapper::play() {

	int result;
	double rms;
	double elapsedMs;
	LARGE_INTEGER perfFreq, perfCnt, newPerfCnt;;

	dse_allocate_frame();
	dse_decode_frame();

	while(dse_feof() == 0) {
		dse_play();
	}

	dse_free_frame();

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
		dse_allocate_frame = (DSEAllocateFrameFunction)
			GetProcAddress(_dse, "dse_allocate_frame");
		dse_decode_frame = (DSEDecodeFrameFunction)
			GetProcAddress(_dse, "dse_decode_frame");
		dse_play = (DSEPlayFunction)
			GetProcAddress(_dse, "dse_play");
		dse_free_frame = (DSEFreeFrameFunction)
			GetProcAddress(_dse, "dse_free_frame");
		dse_get_frame_rms = (DSEGetFrameRMSFunction)
			GetProcAddress(_dse, "dse_get_frame_rms");
		dse_feof = (DSEIsEndOfFileFunction)
			GetProcAddress(_dse, "dse_is_eof");
		dse_close_input = (DSECloseInputFunction)
			GetProcAddress(_dse, "dse_close_input");
	#endif

}

