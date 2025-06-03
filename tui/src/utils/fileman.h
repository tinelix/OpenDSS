/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This file is part of Tinelix OpenDSS.
 *
 *  Tinelix OpenDSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Tinelix OpenDSS is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Tinelix OpenDSS. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef _MSVC		/* If it turns out that we are building a 
                       project using Microsoft Visual Studio. */
	#ifdef __TINYDIR
			#include <tinydir/tinydir.h>
	#else
			#include <framedir/include/framedir.h>
	#endif
#else
	#ifdef _TINYDIR
			#include <tinydir/tinydir.h>
	#else
			#include <framedir/include/framedir.h>
	#endif
#endif

#include <interfaces/filemani.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#ifdef _MSVC
    #include <direct.h>
#else
    #include <unistd.h>
#endif

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

private:
    IFileManager*		gInterface;
    char                gCurrentPath[384];
    int                 gSelectionIndex;
	framedir_file*		gFiles;
    
    long                gFilesCount;
    int                 gPrevSlash;
};

