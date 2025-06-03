#include <platform/windows/dsemodl.h>
#include <stdio.h>

// Loads the library by matching the char* value of the required key

HMODULE dse_win32_load_libs_c(char* dir, int type, char* key, char* value) {
    WIN32_FIND_DATAA wfd;
    HANDLE hFind;
    char full_path[MAX_PATH], module_name[MAX_PATH];
    HMODULE lib_handle;
    FARPROC libtype_proc;
    FARPROC key_proc;
    BOOL isModuleSupported;

    #ifdef _MSVC2005G
        sprintf_s(full_path, MAX_PATH, "%s\\*.dll", dir);
    #else
        sprintf(full_path, "%s\\*.dll", dir);
    #endif // _MSVC2005G
    
    hFind = FindFirstFileA(full_path, &wfd);
    if (hFind == INVALID_HANDLE_VALUE)
        return NULL;

    // Searching and checking DLLs

    do {
        #ifdef _MSVC2005G
            sprintf_s(module_name, MAX_PATH, "%s\\%s", dir, wfd.cFileName);
        #else
            sprintf(module_name, "%s\\%s", dir, wfd.cFileName);
        #endif

        lib_handle = LoadLibraryExA(module_name, NULL, DONT_RESOLVE_DLL_REFERENCES);
        if (lib_handle != NULL) {
            libtype_proc = 
                GetProcAddress(lib_handle, "DSE_GetModuleType"); // Entry point for DSE modules
            if (libtype_proc != NULL) {
                int lib_type = ((int (*)())libtype_proc)();
                if (lib_type != 0 && lib_type == type) {
                    key_proc =
                        GetProcAddress(lib_handle, key);
                    char* key_value = ((char* (*)())key_proc)();
                    if (key_value != NULL && strstr(key_value, value) != NULL) {
                        FreeLibrary(lib_handle);
                        return LoadLibraryA(module_name);
                    } else {
                        return NULL;
                    }
                }
            }
            FreeLibrary(lib_handle);
        }
    } while (FindNextFileA(hFind, &wfd));

    FindClose(hFind);
    return NULL;
}

// Loads the library by matching the BOOL value of the required key

HMODULE dse_win32_load_libs_b(char* dir, int type, char* key, BOOL value) {
    WIN32_FIND_DATAA wfd;
    HANDLE hFind;
    char full_path[MAX_PATH], module_name[MAX_PATH];
    HMODULE lib_handle;
    FARPROC libtype_proc;
    FARPROC key_proc;
    BOOL isModuleSupported;

#ifdef _MSVC2005G
    sprintf_s(full_path, MAX_PATH, "%s\\*.dll", dir);
#else
    sprintf(full_path, "%s\\*.dll", dir);
#endif // _MSVC2005G

    hFind = FindFirstFileA(full_path, &wfd);
    if (hFind == INVALID_HANDLE_VALUE)
        return NULL;

    // Searching and checking DLLs

    do {
        #ifdef _MSVC2005G
            sprintf_s(module_name, MAX_PATH, "%s\\%s", dir, wfd.cFileName);
        #else
            sprintf(module_name, "%s\\%s", dir, wfd.cFileName);
        #endif

        lib_handle = LoadLibraryExA(module_name, NULL, DONT_RESOLVE_DLL_REFERENCES);
        if (lib_handle != NULL) {
            libtype_proc =
                GetProcAddress(lib_handle, "DSE_GetModuleType"); // Entry point for DSE modules
            if (libtype_proc != NULL) {
                int lib_type = ((int (*)())libtype_proc)();
                if (lib_type != 0 && lib_type == type) {
                    key_proc =
                        GetProcAddress(lib_handle, key);
                    BOOL key_value = ((BOOL (*)())key_proc)();
                    if (key_value) {
                        FreeLibrary(lib_handle);
                        return LoadLibraryA(module_name);
                    } else {
                        return NULL;
                    }
                }
            }
            FreeLibrary(lib_handle);
        }
    } while (FindNextFileA(hFind, &wfd));

    FindClose(hFind);
    return NULL;
}