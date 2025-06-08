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

#include <dse.h>
#include <platform/windows/dsewin.h>
#include <platform/windows/dsemodl.h>
#include <Windows.h>

dse_source *dse;
HWAVEOUT hWaveOut;
HMODULE decoder;
unsigned char* pcm_buf_uc;
short** pcm_buf_ss;

HMODULE dse_win32_load_decoder();

typedef int (WINAPI* DSEDecOpenInputFileFunction) (const char*);
typedef int (WINAPI* DSEDecDecodeFrameUCFunction) (unsigned char**, size_t);
typedef int (WINAPI* DSEDecDecodeFrameSSFunction) (short**, size_t);
typedef int (WINAPI* DSEDecCloseInputFileFunction) ();

DSEDecDecodeFrameUCFunction	    DSEDec_DecodeFrameUC;
DSEDecDecodeFrameSSFunction	    DSEDec_DecodeFrameSS;
DSEDecOpenInputFileFunction	    DSEDec_OpenInputFile;
DSEDecCloseInputFileFunction	DSEDec_CloseInputFile;

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

    int result = -1;

    decoder = dse_win32_load_decoder("decoders", "*.mp3");

    if(decoder != NULL) {
        result = 0;
    }

    return result;
}

int dse_win32_prepare(DSE_PCM_OUTPUT_FORMAT out) {

    int result = 0;

    DSE_WAVE_OUTPUT_FORMAT wf;

    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nSamplesPerSec = out.sample_rate;
    wf.wBitsPerSample = out.bits_per_sample;
    wf.nChannels = out.channels;
    wf.nBlockAlign = wf.nChannels * (wf.wBitsPerSample / 8);
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

    result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, NULL, NULL, CALLBACK_NULL);

    return result;
}

int dse_win32_open_input(const char path[512]) {
    int result = dse_win32_init();

    decoder = dse_win32_load_decoder();

    if (decoder != NULL) {
        result = DSEDec_OpenInputFile(dse_file_path);
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
        decoder = _dse_win32_load_decoder("decoders", file_ext);
        if (decoder != NULL); {
            dse_win32_import_decoder();
        }
        return decoder;
    }

    return NULL;
}

void dse_win32_import_decoder() {
    DSEDec_OpenInputFile = (DSEDecOpenInputFileFunction)
        GetProcAddress(decoder, "DSE_OpenInputFile");
    DSEDec_DecodeFrameUC = (DSEDecDecodeFrameUCFunction)
        GetProcAddress(decoder, "DSE_DecodeFrameUC");
    DSEDec_DecodeFrameSS = (DSEDecDecodeFrameSSFunction)
        GetProcAddress(decoder, "DSE_DecodeFrameSS");
    DSEDec_CloseInputFile = (DSEDecCloseInputFileFunction)
        GetProcAddress(decoder, "DSE_CloseInputFile");
}

int dse_win32_close_input() {
    return DSEDec_CloseInputFile();
}

int dse_win32_decode_frame() {
    int result;
    WAVEHDR hdr;
    int decoder_type = 0;

    pcm_buf_uc = (unsigned char**)malloc((4096 + 1) * sizeof(unsigned char));
    result = DSEDec_DecodeFrameUC(pcm_buf_uc, 4096);

    if (result == -101) {
        decoder_type = 1;
        pcm_buf_ss = (short**)malloc((4096 + 1) * sizeof(short));
        result = DSEDec_DecodeFrameSS(pcm_buf_ss, 4096);
        if (result < 0) {
            return result;
        }
        hdr.lpData = (LPSTR)pcm_buf_ss;
    } else {
        hdr.lpData = (LPSTR)pcm_buf_uc;
    }
    hdr.dwBufferLength = result;
    hdr.dwFlags = 0;
    hdr.dwLoops = 0;
    hdr.dwUser = 0;

    waveOutPrepareHeader(hWaveOut, &hdr, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &hdr, sizeof(WAVEHDR));
    while ((hdr.dwFlags & WHDR_DONE) == 0) {
        Sleep(10);
    }
    waveOutUnprepareHeader(hWaveOut, &hdr, sizeof(WAVEHDR));
    
    if (decoder_type == 1) {
        if (pcm_buf_ss)
            free(pcm_buf_ss);
    } else {
        if (pcm_buf_uc)
            free(pcm_buf_uc);
    }

}