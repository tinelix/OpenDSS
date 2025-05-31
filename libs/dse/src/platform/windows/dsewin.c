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

HMODULE dse_win32_load_decoder();

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

    decoder = dse_win32_load_decoder();

    if(decoder != NULL) {
        result = 0;
    }

    return result;
}

int dse_win32_prepare(DSE_PCM_OUTPUT_PARAMS out_params) {

    int result = 0;

    DSE_WAVE_OUTPUT_FORMAT wf;

    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 1;
    wf.nSamplesPerSec = out_params.sample_rate;
    wf.nAvgBytesPerSec = out_params.sample_rate * sizeof(short);
    wf.nBlockAlign = sizeof(short);
    wf.wBitsPerSample = out_params.bits_per_sample;

    result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, NULL, NULL, CALLBACK_NULL);

    return result;
}

int dse_win32_open_src(char path[512]) {
    int result = dse_win32_init();

    return result;
}

HMODULE dse_win32_load_decoder() {
    char* file_ext;
    HMODULE decoder;

    if (dse_file_path != NULL) {
        file_ext = getfext(dse_file_path);
        decoder = dse_win32_load_libs_c("decoders", 1, "DSE_GetSupportedFormats", file_ext);
        return decoder;
    }

    return NULL;
}