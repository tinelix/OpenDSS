#include "fileman.h"

FileManager::FileManager(IFileManager *interface) {
    gInterface = interface;
}

void FileManager::readCurrentDir() {
    int object_index = 0;
    char cwd[255];
    DIR *dir;
    struct dirent *ent;
    if (getcwd(cwd, sizeof(cwd))) {
        if ((dir = opendir(cwd)) != NULL) {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) {
                gInterface->onDirectoryRead(ent, object_index);
                object_index++;
            }
            closedir (dir);
        } else {
            gInterface->onError(0, 0);
        }
    } else {
        gInterface->onError(0, 0);
    }
}
