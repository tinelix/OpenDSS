#ifndef _OPENDSS_WINDOWS_FILEMANW_H
#define _OPENDSS_WINDOWS_FILEMANW_H

#include <controls/extwnd.h>
#include <utils/fileman.h>

#include <stdio.h>
#include <stdlib.h>

class IFileSelector  : public IFileManager {
    public:
        void setFileManager(ExtWindowCtrl* pWindow, FileManager* pFileMan);
        void onError(char* path, int cmdId, int errorCode);
        void onResult(char* path, int cmdId, int resultCode);
        void onDirectoryRead(char* path, framedir_file* files);
    private:
        ExtWindowCtrl*  hWindow;
        FileManager*    hFileMan;
};

class FileManagerWnd : public ExtWindowCtrl {
    public:
        FileManagerWnd();
        void            createWnd();
        WINDOW*         getWnd();
        void            prepare();
        void            readCurrentDir();
        void            listenKbd();
        void            onFileSelectorSuccess();
        char            id[60];
    private:
        void            onKeyPressed(char key);
        bool            hKbdListening;
        FileManager*    hFileMan;
        IFileSelector*  hInterface;
};

#endif