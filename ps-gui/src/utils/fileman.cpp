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

#include "fileman.h"

#define MAX_FILES_COUNT 2048

FileManager::FileManager(IFileManager* pInterface) {
    gInterface = pInterface;
    gFiles = (tinydir_file*)malloc(MAX_FILES_COUNT * sizeof(tinydir_file));
    gFilesCount = 0;
}

FileManager::~FileManager() {
    free(gFiles);
}

/* Reads working directory and shows listing */

void FileManager::readCurrentDir() {
    char cwd[384];
    #ifdef _MSVC
        if (_getcwd(cwd, sizeof(cwd)) != NULL) {
    #else
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
    #endif
        readDir(cwd);
    }
    else
        gInterface->onError(0, 1);
}

void FileManager::readDir(char* pDirPath) {
    int object_index = 0;
    tinydir_dir dir;

    #ifdef _MSVC2005G
        sprintf_s(gCurrentPath, "%s", pDirPath);
    #else
        sprintf(gCurrentPath, "%s", pDirPath);
    #endif

    #ifdef _MSVC
        if (tinydir_open_sorted(&dir, (const _tinydir_char_t*)pDirPath) == -1) {
    #else
        if (tinydir_open_sorted(&dir, pDirPath) == -1) {
    #endif
        gInterface->onError(0, -1);
        return;
    }

    for (int i = 1; i < dir.n_files; i++) {
        if (object_index >= MAX_FILES_COUNT) {
            break;
        }
        tinydir_file file;
        if (tinydir_readfile_n(&dir, &file, i) == -1) {
            continue;
        }
        gFiles[object_index] = file;
        object_index++;
    }

    gFilesCount = object_index;
    gInterface->onDirectoryRead(gFiles);
    tinydir_close(&dir);
}

tinydir_file FileManager::getFile(int index) {
    return gFiles[index];
}

long FileManager::getFilesCount() {
    return gFilesCount;
}

char* FileManager::getRealPath(char* pDirPath) {
    int lastPathSlash = 0;
    int pathSlashesCount = -1;

    int pathSlashes[192];

    int pathLen = strlen(pDirPath);

    for (int i = 0; i < pathLen; i++) {
        if (pDirPath[i] == '/') {
            pathSlashesCount++;
            pathSlashes[pathSlashesCount] = i;
        }
    }

    if (pathSlashesCount > 1) {
        if (
            pDirPath[pathLen - 3] == '/' && pDirPath[pathLen - 2] == '.'
            && pDirPath[pathLen - 1] == '.'
            ) {
            lastPathSlash = pathSlashes[pathSlashesCount - 2];
            pDirPath[lastPathSlash] = '\0';
        }
    }
    else if (pathSlashesCount == 1) {
        if (
            pDirPath[pathLen - 3] == '/' && pDirPath[pathLen - 2] == '.'
            && pDirPath[pathLen - 1] == '.'
            )
            pDirPath[lastPathSlash] = '\0';
    }
    return pDirPath;
}

char* FileManager::getCurrentPath() {
    return gCurrentPath;
}
