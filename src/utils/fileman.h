/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU General Public License 3 (or any later version) and/or Apache License 2
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */

#pragma once

#include <tinydir.h>
#include <interfaces/fileman.h>
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
    tinydir_file getFile(int index);
    long getFilesCount();
    char* getRealPath(char* pDirPath);
    char* getCurrentPath();

private:
    IFileManager* gInterface;
    char                gCurrentPath[384];
    int                 gSelectionIndex;
    tinydir_file* gFiles;
    long                gFilesCount;
    int                 gPrevSlash;
};

