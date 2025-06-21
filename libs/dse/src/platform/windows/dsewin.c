/*  Digital Sound Engine - basis of Tinelix OpenDSS
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU Lesser General Public License 2.1 (or any later version) and/or BSD 3-Clause
 *  License.
 *
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.BSD
 *     LICENSE.LGPL
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */
#include <platform/windows/dsewin.h>
#include <platform/windows/dsemodl.h>
#include <Windows.h>

dse_source *dse;

HWAVEOUT hWaveOut;
HMODULE decoder;

DSE_USER_DATA dse_user_data;
DSE_AUDIO_OUTPUT_INFO dse_audio_out;

double dse_current_frame_rms;
int frames_count = 0;
int is_riff = 0;
WAVEHDR buffers[6];
HANDLE hEvent;
int buffer_idx = 0;
DWORD remainingSmp = 4294967295;

HMODULE dse_win32_load_decoder();
void dse_win32_import_decoder();

BOOL CALLBACK DSE_WaveOutCallback(
	HWAVEOUT hWaveOut,
	UINT uMsg,
	DWORD uParam,
	DWORD lParam,
	DWORD pParam
);

typedef int (WINAPI* DSEDecOpenInputFileFunction) (const char*);
typedef int (WINAPI* DSEDecDecodeFrameU8Function) (unsigned char**, size_t);
typedef int (WINAPI* DSEDecDecodeFrameS16LEFunction) (short**, size_t);
typedef DSE_AUDIO_OUTPUT_INFO (WINAPI* DSEDecGetOutputInfoFunction) ();
typedef int (WINAPI* DSEDecGetStreamInfoFunction) (DSE_STREAM_INFO* info);
typedef int (WINAPI* DSEDecIsEndOfFileFunction) ();
typedef int (WINAPI* DSEDecCloseInputFileFunction) ();

DSEDecDecodeFrameU8Function			DSEDec_DecodeFrameU8;
DSEDecDecodeFrameS16LEFunction	    DSEDec_DecodeFrameS16LE;
DSEDecOpenInputFileFunction			DSEDec_OpenInputFile;
DSEDecGetOutputInfoFunction			DSEDec_GetOutputInfo;
DSEDecGetStreamInfoFunction			DSEDec_GetStreamInfo;
DSEDecIsEndOfFileFunction			DSEDec_IsEndOfFile;
DSEDecCloseInputFileFunction		DSEDec_CloseInputFile;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int dse_win32_init(void) {

    int result = 0;

    return result;
}

int dse_win32_prepare(DSE_AUDIO_OUTPUT_INFO out) {

    int result = 0;

    DSE_WAVE_OUTPUT_FORMAT wf;

	dse_audio_out = out;

    wf.wFormatTag		= WAVE_FORMAT_PCM;
    wf.nSamplesPerSec	= out.sample_rate;
    wf.wBitsPerSample	= out.bits_per_sample;
    wf.nChannels		= out.channels;
    wf.nBlockAlign		= wf.nChannels * (wf.wBitsPerSample / 8);
    wf.nAvgBytesPerSec	= (wf.nSamplesPerSec * wf.nBlockAlign);


	dse_user_data.buffer_size	= (wf.nSamplesPerSec * wf.nBlockAlign) / 6;
	dse_user_data.header		= NULL;
	dse_user_data.next_header	= NULL;

    result = waveOutOpen(
		&hWaveOut, WAVE_MAPPER, &wf, 
		#ifdef _MSVC2005G
			(DWORD_PTR)DSE_WaveOutCallback, 
		#else
			(DWORD)DSE_WaveOutCallback, 
		#endif
		0, CALLBACK_FUNCTION);

	frames_count = 0;
	buffer_idx = 0;
	
	if (result != MMSYSERR_NOERROR) {
		return -1;
	}

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    return result;
}

int dse_win32_open_input(const char path[512]) {
	int result, prepare_result, info_result;

    decoder = dse_win32_load_decoder();

    if (decoder != NULL) {
        result = DSEDec_OpenInputFile(dse_file_path);
		out_info = DSEDec_GetOutputInfo();
		prepare_result = dse_win32_prepare(out_info);
    } else if(is_riff == 1) {
        result = DSE_RIFF_OpenInputFile(dse_file_path);
		info_result = DSE_RIFF_GetOutputInfo(&out_info);
		prepare_result = dse_win32_prepare(out_info);
    } else {
        result = -2;
    }

    return result;
}

HMODULE dse_win32_load_decoder() {
    char* file_ext;
    HMODULE decoder;

    if (dse_file_path != NULL) {
        file_ext = getfext(dse_file_path);

		if(!file_ext) {
			return NULL;
		}

		if(strcmp(file_ext, "wav") == 0) {
			is_riff = 1;
			return NULL;
		} else {
			is_riff = 0;
			decoder = _dse_win32_load_decoder("decoders", file_ext);
			if (decoder != NULL); {
				dse_win32_import_decoder();
			}
			return decoder;
		}
    }
	
    return NULL;
}

void dse_win32_import_decoder() {
    DSEDec_OpenInputFile = (DSEDecOpenInputFileFunction)
        GetProcAddress(decoder, "DSE_OpenInputFile");
    DSEDec_DecodeFrameU8 = (DSEDecDecodeFrameU8Function)
        GetProcAddress(decoder, "DSE_DecodeFrameU8");
    DSEDec_DecodeFrameS16LE = (DSEDecDecodeFrameS16LEFunction)
        GetProcAddress(decoder, "DSE_DecodeFrameS16LE");
	DSEDec_GetStreamInfo = (DSEDecGetStreamInfoFunction)
        GetProcAddress(decoder, "DSE_GetStreamInfo");
	DSEDec_IsEndOfFile = (DSEDecIsEndOfFileFunction)
        GetProcAddress(decoder, "DSE_IsEndOfFile");
    DSEDec_CloseInputFile = (DSEDecCloseInputFileFunction)
        GetProcAddress(decoder, "DSE_CloseInputFile");
}

void dse_win32_allocate_frame() {
	int i = 0;
	buffer_idx = 0;

	for(i + 0; i < 6; i++) {
		buffers[i].lpData = malloc((dse_user_data.buffer_size)); 
		buffers[i].dwFlags			= WHDR_BEGINLOOP | WHDR_ENDLOOP;
		buffers[i].dwLoops			= 1;
		buffers[i].dwUser			= 0;
		buffers[i].lpNext			= NULL;
		buffers[i].reserved			= 0;
		buffers[i].dwBytesRecorded	= 0;
	}

}

int dse_win32_decode_frame() {
	int result;
	int i = 0;
    int decoder_type = 0;
	HANDLE eventHandle;
	DWORD waitResult;

	if(out_info.bits_per_sample == 8) {
		buffers[buffer_idx].dwBufferLength = (dse_user_data.buffer_size);

		if(is_riff == 1) {
			result = DSE_RIFF_DecodeFrameU8(
				&buffers[buffer_idx].lpData, dse_user_data.buffer_size
			);
		} else {
			result = DSEDec_DecodeFrameU8(
				&buffers[buffer_idx].lpData, dse_user_data.buffer_size
			);
		}
	} else {
		buffers[buffer_idx].dwBufferLength = (dse_user_data.buffer_size / 2);

		if(is_riff == 1) {
			result = DSE_RIFF_DecodeFrameS16LE(
				&buffers[buffer_idx].lpData, dse_user_data.buffer_size
			);
		} else {
			result = DSEDec_DecodeFrameS16LE(
				&buffers[buffer_idx].lpData, dse_user_data.buffer_size
			);
		}
	}

	frames_count++;
}

void dse_win32_play() {
	int result = 0;
	MMTIME positionInfo = {0};
	char debug[320];
	MMRESULT mmresult;

	positionInfo.wType = TIME_SAMPLES;

	if (!(buffers[0].dwFlags & WHDR_PREPARED)) {
		buffers[0].dwFlags |= WHDR_PREPARED;
		waveOutPrepareHeader(hWaveOut, &buffers[0], sizeof(WAVEHDR));
	}

	result = waveOutGetPosition(hWaveOut, &positionInfo, sizeof(MMTIME));

	if (result == MMSYSERR_NOERROR) {
			remainingSmp = 
				(positionInfo.u.sample <= buffers[buffer_idx].dwBufferLength) ?
                (buffers[buffer_idx].dwBufferLength - positionInfo.u.sample) :
                0;
	}

	waveOutWrite(hWaveOut, &buffers[buffer_idx], sizeof(WAVEHDR));

	if(out_info.bits_per_sample == 16) {
		dse_current_frame_rms = dse_win32_calculate_rms_s16le(
			buffers[buffer_idx].lpData, dse_user_data.buffer_size
		);
	} else {
		dse_current_frame_rms = dse_win32_calculate_rms_u8(
			buffers[buffer_idx].lpData, dse_user_data.buffer_size
		);
	}

	buffer_idx++;
	buffer_idx %= 6;

	Sleep(20);

	dse_win32_decode_frame();
}

double dse_win32_calculate_rms_u8(
	const unsigned char *samples, int samples_size
) {
    double squared_sum = 0.0;
	double squared_mean = 0.0;
	int i;
    
    for(i = 0; i < samples_size; ++i) {
        squared_sum += samples[i] * samples[i];
    }
    
    squared_mean = squared_sum / samples_size;

	return sqrt(squared_mean);
}

double dse_win32_get_frame_rms() {
	return dse_current_frame_rms;
}

double dse_win32_calculate_rms_s16le(
	const short *samples, int samples_size
) {
    return 128.0;
}

int dse_win32_get_stream_info(DSE_STREAM_INFO* info) {

	if(is_riff == 1) {
		return DSE_RIFF_GetStreamInfo(info);
	} else {
		return DSEDec_GetStreamInfo(info);
	}
}

void dse_win32_free_frame() {
	int i;

	char debug[250];

	for(i + 0; i < 6; i++) {
		if(buffers[i].lpData != NULL) {
			waveOutUnprepareHeader(hWaveOut, &buffers[i], sizeof(WAVEHDR));
			free(buffers[i].lpData);
			buffers[i].lpData = NULL;
		}
	}
}

int dse_win32_is_eof() {
	if(is_riff == 1)
		return DSE_RIFF_IsEndOfFile();
	else
		return DSEDec_IsEndOfFile();
}

int dse_win32_close_input() {

	if(dse_user_data.playing == 0) return;

	dse_user_data.playing = 0;

    return DSEDec_CloseInputFile();
}

BOOL CALLBACK DSE_WaveOutCallback(
	HWAVEOUT hWaveOut,
	UINT uMsg,
	DWORD uParam,
	DWORD lParam,
	DWORD pParam
) {

	int i;
	int result;

	switch(uMsg)
    {
        case MM_WOM_DONE:
			SetEvent(hEvent);  
            break;
    }
    return TRUE;

}
