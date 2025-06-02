#ifndef _FRAMEDIR_H
#define _FRAMEDIR_H

#define FRAMEDIR_PATH_MAX	4096
#define FRAMEDIR_FN_MAX		 256

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
	#include <windows.h>
	#include <tchar.h>
#endif

typedef struct {
	char path[FRAMEDIR_PATH_MAX];
	char name[FRAMEDIR_FN_MAX];
	char *ext;
	int is_dir;
	int is_dev;
} framedir_file;

typedef struct {
	char path[FRAMEDIR_PATH_MAX];
	size_t n_files;
	framedir_file *_files;
	#ifdef _WIN32
		HANDLE _h;
		WIN32_FIND_DATA _f;
	#endif
} framedir_dir;

int framedir_open(framedir_dir* dir, const char* path);

int framedir_readfile_n(
	const framedir_dir* dir, 
	framedir_file *file, 
	int index
);

char* framedir_normalize_path(const char* path);

void framedir_close(framedir_dir* dir);

#ifdef __cplusplus
}
#endif

#endif