#ifndef DSE_WINDOWS_DSEMODL_H
#define DSE_WINDOWS_DSEMODL_H

#define MAX_SUPPORTED_MEDIA_FORMATS 5

#include <Windows.h>
#include <utils/fileext.h>

HMODULE dse_win32_load_libs_c(char* dir, int type, char* key, char* value);
HMODULE dse_win32_load_libs_b(char* dir, int type, char* key, BOOL value);

#endif
