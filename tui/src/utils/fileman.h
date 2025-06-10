/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  ----------------------------------------------------------------------------
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
 *  information, (in the opening comment of each file).
 */

#pragma once

#include <framedir/include/framedir.h>
#include <interfaces/filemani.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#ifdef _MSVC
    #include <direct.h>
#else
    #include <unistd.h>
#endif

// Forbidden file extensions

static const char* forbidden_fexts[] = {
	// DOS/Windows executables
    "com", "exe", "dll","ax",

	// Microsoft Visual C++ files
	"vcsproj", "ilk", "obj",

	// ELF files (UNIX and etc.)
	"elf", "o", "so",

	// Graphics files (unsupported yet)
    "png", "jpg", "gif", "heic",

	// Video files (unsupported yet)
	"heif", "mov", "mp4", "mpg", "vob",

	// Archive files
    "tar.gz", "zip", "rar", "7z",

	// Image files (unsupported yet)
	"iso", "img", "fdd", "ima", "fdi"
};

#define FORBIDDEN_FEXT_COUNT (sizeof(forbidden_fexts)/sizeof(forbidden_fexts[0]))

class FileManager {
public:
    FileManager(IFileManager* pInterface);
    ~FileManager();
    void readCurrentDir();
    void readDir(char* pDirPath);
	void allocateFilesArray();
	void freeFilesArray();

	#ifdef _MSVC		/* If it turns out that we are building a 
                       project using Microsoft Visual Studio. */
		#ifdef __TINYDIR
		    tinydir_file getFile(int index);
		#else
		    framedir_file getFile(int index);
		#endif
	#else
		#ifdef _TINYDIR
			tinydir_file getFile(int index);
		#else
			framedir_file getFile(int index);
		#endif
	#endif

    long getFilesCount();
    char* getRealPath(char* pDirPath);
    char* getCurrentPath();
	int checkFileExtInBlacklist(char* fpath);

private:
    IFileManager*		gInterface;
    char                gCurrentPath[384];
    int                 gSelectionIndex;
	framedir_file*		gFiles;
    
    long                gFilesCount;
    int                 gPrevSlash;
};

