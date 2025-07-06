/*  FrameDir - TinyDir alternative with support for older Windows versions
 *  ----------------------------------------------------------------------------
 *  Copyright © 2024, 2025 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This library is free software: you can redistribute it and/or modify it 
 *  under the terms of the GNU Lesser General Public License 3 (or any later 
 *  version) and/or Apache License 2.
 *
 *  See the following files in repository directory for the precise terms and 
 *  conditions of either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.LGPLv3
 *
 *  Please see each file in the implementation for copyright and licensing 
 *  information, (in the opening comment of each file).
 */

#ifndef _FRAMEDIR_H
#define _FRAMEDIR_H

#ifdef _DOS
	#define FRAMEDIR_PATH_MAX	  81
	#define FRAMEDIR_FN_MAX		  12
#else
	#define FRAMEDIR_PATH_MAX	4096
	#define FRAMEDIR_FN_MAX		 256
#endif

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
	size_t i_files;
	framedir_file *_files;
	int allocated;
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

char* framedir_get_fileext(const char* fpath);

void framedir_close(framedir_dir* dir);

#ifdef __cplusplus
}
#endif

#endif
