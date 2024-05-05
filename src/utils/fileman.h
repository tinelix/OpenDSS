#ifndef OPENDSS_UTILS_FILEMAN_H
#define OPENDSS_UTILS_FILEMAN_H

#include <dirent.h>
#include <unistd.h>
#include <interfaces/fileman.h>

class FileManager {
    public:
        FileManager(IFileManager *interface);
        void readCurrentDir();
    private:
        IFileManager *gInterface;
};

#endif
