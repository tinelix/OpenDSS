#ifndef OPENDSS_WINDOWS_FILEMAN_H
#define OPENDSS_WINDOWS_FILEMAN_H

#include <stdio.h>
#include <stdlib.h>
#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#else
    #include <ncurses.h>
#endif
#include <dirent.h>
#include <wchar.h>

#ifdef __MINGW64__
    #include <nstddef.h>
    #include <sys/stat.h>
#endif

#include "../controls/uictrl.h"
#include "../controls/extwnd.h"
#include "../controls/listbox.h"
#include "../controls/msgbox.h"

#include "../utils/audtags.h"
#include "../utils/fileman.h"

#include "../interfaces/fileman.h"


class FileManagerWnd : public ExtWindowCtrl {
    public:
        FileManagerWnd(FileManager *pFileMan, IFileManager *pInterface);
        char* getSelectedFileName();
        void onKeyPressed(char k);
        void onDirectoryRead(dirent **ents);
        void onFileManResult(int cmdId, int resultCode);
        void onFileManError(int cmdId, int errorCode);
        bool disableListening;
    private:
        FileManager     *gFileMan;
        IFileManager    *gInterface;
        char            *gSelectedFileName;
};
#endif
