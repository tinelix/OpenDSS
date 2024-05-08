#include "fileman.h"
#include <cstring>
#include <curses.h>

FileManager::FileManager(IFileManager *interface) {
    gInterface = interface;
    gSelectionIndex = -1;
    gEnts = (dirent**)malloc(640 * sizeof(dirent));
}

FileManager::~FileManager() {
    free(gEnts);
}

/* Reads working directory and shows listing */

void FileManager::readCurrentDir() {
    char cwd[255];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        readDir(cwd);
    } else
        gInterface->onError(0, 1);
}

void FileManager::readDir(char* pDirPath) {
    int object_index = 0;
    DIR *dir;
    struct dirent *ent;
    if (pDirPath) {
        if ((dir = opendir(pDirPath)) != NULL) {    // if this directory exist
            sprintf(gCurrentPath, "%s", pDirPath);
            object_index = 0;
            gInterface->onResult(0, 1);
            /* Print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) {
                if(object_index >= 640) {
                    break;
                } else if(strcmp(ent->d_name, ".") == 0) {
                    /* Excludes '.' from the list, since opening this
                       object returns the same directory. */
                    continue;
                }
                gEnts[object_index] = ent;
                object_index++;
            }
            gFilesCount = object_index;
            gInterface->onDirectoryRead(gEnts);
            closedir (dir);
        } else {
            gInterface->onError(0, 2);
        }
    } else {
        gInterface->onError(0, 1);
    }
}

dirent* FileManager::getFile(int index) {
    return gEnts[index];
}

long FileManager::getFilesCount() {
    return gFilesCount;
}

char* FileManager::getCurrentPath() {
    return gCurrentPath;
}
