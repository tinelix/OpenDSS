#ifndef OPENDSS_UTILS_FILEMAN_H
#define OPENDSS_UTILS_FILEMAN_H

#include <dirent.h>
#include <unistd.h>
#include <interfaces/fileman.h>
#include <stdio.h>
#include <stdlib.h>

class FileManager {
    public:
        FileManager(IFileManager *interface);
        ~FileManager();
        void readCurrentDir();
        dirent* getFile(int index);
        int getSelectionIndex();
        void setSelectionIndex(int index);
        long getFilesCount();
    private:
        IFileManager *gInterface;
        char         *gCurrentPath;
        int          gSelectionIndex;
        dirent       **gEnts;
        long         gFilesCount;
};

#endif
