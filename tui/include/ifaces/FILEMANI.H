#ifndef _OPENDSS_IFACES_FILEMANI_H
#define _OPENDSS_IFACES_FILEMANI_H

#include <framedir/include/framedir.h>

class IFileManager {
    public:
        IFileManager() {};
        virtual ~IFileManager() {};
        virtual void onError(char* path, int cmdId, int errorCode) = 0;
        virtual void onResult(char* path, int cmdId, int resultCode) = 0;
        virtual void onDirectoryRead(char* path, framedir_file* files) = 0;
};

#endif