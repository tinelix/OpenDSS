#ifndef OPENDSS_INTERFACES_FILEMAN_H
#define OPENDSS_INTERFACES_FILEMAN_H

#include <dirent.h>

class IFileManager {
    public:
        IFileManager() {};
        virtual ~IFileManager() {};
        virtual void onError(int cmdId, int errorCode) = 0;
        virtual void onResult(int cmdId, int resultCode) = 0;
        virtual void onDirectoryRead(struct dirent* ent, int index) = 0;
};

#endif
