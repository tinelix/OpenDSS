#ifndef _OPENDSS_UTILS_FILEMAN_H
#define _OPENDSS_UTILS_FILEMAN_H

#include <framedir/include/framedir.h>
#include <ifaces/filemani.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSVC
    #include <direct.h>
#else
    #include <unistd.h>
#endif

class FileManager {
    public:
        FileManager(IFileManager* pInterface);
        ~FileManager();
        int             readCurrentDir();
        int             readDir(char* pDirPath);
        framedir_file   getFile(int index);
        long            getFilesCount();
        char*           getRealPath(char* pDirPath);
        char*           getCurrentPath();
        void            allocateFilesArray();
        void            freeFilesArray();

    private:
        IFileManager*   hInterface;
        char*           hCurrentPath;
        int             hSelectionIndex;
        framedir_file*  hFiles;
        long            hFilesCount;
        int             hPrevSlash;
};

#endif