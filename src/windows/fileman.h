/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU General Public License 3 (or any later version) and/or Apache License 2
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */

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
