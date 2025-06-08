#ifndef DSE_WINDOWS_DSEMODL_H
#define DSE_WINDOWS_DSEMODL_H

#define MAX_SUPPORTED_MEDIA_FORMATS 5

#include <Windows.h>
#include <utils/fileext.h>

HMODULE _dse_win32_load_decoder(char* dir, char* f_ext);
void dse_win32_import_decoder();

#endif
