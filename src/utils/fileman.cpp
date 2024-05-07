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
    int object_index = 0;
    char cwd[255];
    DIR *dir;
    struct dirent *ent;
    if (getcwd(cwd, sizeof(cwd))) {
        gCurrentPath = cwd;
        if ((dir = opendir(cwd)) != NULL) {
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
                gInterface->onDirectoryRead(ent, object_index);
                object_index++;
            }
            gFilesCount = object_index;
            closedir (dir);
        } else {
            gInterface->onError(0, 0);
        }
    } else {
        gInterface->onError(0, 0);
    }
}

dirent* FileManager::getFile(int index) {
    return gEnts[index];
}

long FileManager::getFilesCount() {
    return gFilesCount;
}
