/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------------
 *  Copyright © 2024, 2025 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License 3 (or any later version)
 *  and/or Apache License 2.
 * 
 *  See the following files in repository directory for the precise terms and
 *  conditions of either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing
 *  information (in the opening comment of each file).
 */

#pragma once

#define OPENDSS_VERSION "0.0.1"

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <ntstatus.h>

    typedef LONG NTSTATUS, * PNTSTATUS;
    #define STATUS_SUCCESS (0x00000000)

    typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
#endif

class OpenDSSVersion {
public:
    static char* getVersion() {
        char* version = new char[24];
        #ifdef _MSVC2005G
            #ifdef OPENDSS_SUFFIX
                sprintf_s(version, 24, "%s", OPENDSS_VERSION);
            #else
                sprintf_s(version, 24, "%s", OPENDSS_VERSION);
            #endif
        #else
            #ifdef OPENDSS_SUFFIX
                sprintf(version, "%s", OPENDSS_VERSION);
            #else
                sprintf(version, "%s", OPENDSS_VERSION);
            #endif
        #endif

        return version;
    }

    #ifdef _WIN32
        static int* getWindowsVersion() { // Correct getting Windows OS version
            int* version = new int[3];

            version[0] = -1;
            version[1] = -1;
            version[2] = -1;

            HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");

            if (hMod) {
                RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
                if (fxPtr != NULL) {
                    RTL_OSVERSIONINFOW rovi = { 0 };
                    rovi.dwOSVersionInfoSize = sizeof(rovi);
                    if (STATUS_SUCCESS == fxPtr(&rovi)) {
                        version[0] = rovi.dwMajorVersion;
                        version[1] = rovi.dwMinorVersion;
                        version[2] = rovi.dwBuildNumber;
                    }
                }
            }
            
            return version;
        }
    #endif
};
