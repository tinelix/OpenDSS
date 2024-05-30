#ifndef OPENDSS_UTILS_FILEMAN_H
#define OPENDSS_UTILS_FILEMAN_H

#include <dirent.h>
#include <unistd.h>
#include <interfaces/fileman.h>
#include <controls/listbox.h>
#include <stdio.h>
#include <stdlib.h>

class FileManager {
    public:
        FileManager(IFileManager *interface);
        ~FileManager();
        void countSlashes(char* pDirPath);
        void readCurrentDir();
        void readDir(char* pDirPath);
        dirent* getFile(int index);
        long getFilesCount();
        char* getRealPath(char* pDirPath);
        char* getCurrentPath();

    private:
        IFileManager *gInterface;
        char         gCurrentPath[384];
        int          gSelectionIndex;
        dirent       **gEnts;
        long         gFilesCount;
        int          gPrevSlash;
};

#endif
