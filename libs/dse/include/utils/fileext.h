#ifndef DSE_UTILS_FILEEXT_H
#define DSE_UTILS_FILEEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getfext(const char* filename);
int chkfext(const char* ext_list, const char* ext);

#endif
