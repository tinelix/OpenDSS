#include <platform/windows/dsemodl.h>
#include <stdio.h>

// Loads the library by matching the char* value of the required key

#ifdef _WIN32
    typedef int     (WINAPI* DSEGetModuleTypeFunction) ();
    typedef char*   (WINAPI* DSEGetSupportedMediaFormatsFunction) ();
#endif

DSEGetModuleTypeFunction                DSEGetModuleType;
DSEGetSupportedMediaFormatsFunction     DSEGetSupportedMediaFormats;

HMODULE _dse_win32_load_decoder(char* dir, char* f_ext) {
    WIN32_FIND_DATAA wfd;
    HANDLE hFind;
    char full_path[MAX_PATH], module_name[MAX_PATH];
    HMODULE lib_handle;
    BOOL isModuleSupported;
    char cwd[512];

    #ifdef _MSVC
        if (_getcwd(cwd, sizeof(cwd)) == NULL) {
    #else
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
    #endif
        return NULL;
    }

    #ifdef _MSVC2005G
        sprintf_s(full_path, MAX_PATH, "%s\\libs\\%s\\*.dll", cwd, dir);
    #else
        sprintf(full_path, "%s\\libs\\%s\\*.dll", cwd, dir);
    #endif // _MSVC2005G
    
    hFind = FindFirstFileA(full_path, &wfd);
    if (hFind == INVALID_HANDLE_VALUE)
        return NULL;

    // Searching and checking DLLs

    do {
        #ifdef _MSVC2005G
            sprintf_s(module_name, MAX_PATH, "%s\\libs\\%s\\%s", cwd, dir, wfd.cFileName);
        #else
            sprintf(module_name, "%s\\libs\\%s\\%s", cwd, dir, wfd.cFileName);
        #endif

        lib_handle = LoadLibraryExA(module_name, NULL, DONT_RESOLVE_DLL_REFERENCES);
        if (lib_handle != NULL) {
            DSEGetModuleType =
                GetProcAddress(lib_handle, "DSE_GetModuleType"); // Entry point for DSE modules
            DSEGetSupportedMediaFormats = GetProcAddress(lib_handle, "DSE_GetSupportedMediaFormats");
            if (DSEGetModuleType() == 1) {
                char* supportedFormats = DSEGetSupportedMediaFormats();
                if (supportedFormats != NULL && strstr(supportedFormats, f_ext) != NULL) {
                    FreeLibrary(lib_handle);
                    return LoadLibraryA(module_name);
                } else {
                    return NULL;
                }
            }
            FreeLibrary(lib_handle);
        }
    } while (FindNextFileA(hFind, &wfd));

    FindClose(hFind);
    return NULL;
}