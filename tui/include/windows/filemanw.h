#ifndef _OPENDSS_WINDOWS_FILEMANW_H
#define _OPENDSS_WINDOWS_FILEMANW_H

#include <controls/extwnd.h>
#include <utils/fileman.h>

#include <stdio.h>
#include <stdlib.h>

class FileManagerWnd : public ExtWindowCtrl {
    public:
        FileManagerWnd(/* */);
        void createWnd();
        void readCurrentDir();
        void listenKbd();
    private:
        void onKeyPressed(char key);
        bool hKbdListening;
        FileManager* hFileMan;
};

#endif