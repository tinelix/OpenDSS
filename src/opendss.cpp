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

#include "windows/player.h"
#include <cstring>
#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#else
    #include <ncurses.h>
#endif

#include <stdio.h>                  // Linking standard C functions
#include <tinydir.h>
#include <unistd.h>

#include "utils/audtags.h"
#include "utils/fileman.h"
#include "utils/pguiman.h"
#include "utils/extstr.h"

#include "windows/fileman.h"
#include "windows/player.h"

#include "controls/extwnd.h"
#include "controls/listbox.h"
#include "controls/msgbox.h"

#include "interfaces/fileman.h"
#include "interfaces/pguiman.h"

class IOpenDSSFileManager : IFileManager {
    public:
        void onError(int cmdId, int errorCode);
        void onResult(int cmdId, int resultCode);
        void onDirectoryRead(tinydir_file* files);
};

IOpenDSSFileManager          *gFileManInterface;

class IOpenDSSPseudoGUIManager : IPseudoGUIManager {
    public:
        void onKeyPressed(char k) {};
        void onKeyPressed(char k, ExtWindowCtrl *pExtWnd);
};

IOpenDSSPseudoGUIManager     *gPsGUIManInterface;

PseudoGUIManager                *gPsGuiMan;
FileManager                     *gFileMan;

FileManagerWnd                  *gFileManWnd;

/* Creates File Manager window and shows directory listing. */

void openFileManager() {
    gFileManWnd = new FileManagerWnd(gFileMan, (IFileManager*)gFileManInterface);
    gFileMan->readCurrentDir();
}

/* Application main function */

int main() {
    gPsGUIManInterface = new IOpenDSSPseudoGUIManager();
    gFileManInterface = new IOpenDSSFileManager();

    gFileMan = new FileManager((IFileManager*)gFileManInterface);
    gPsGuiMan = new PseudoGUIManager((IPseudoGUIManager*)gPsGUIManInterface);

    gPsGuiMan->showTopVersionInfo();

    openFileManager();

    delete gPsGuiMan;
    return 0;
}

void openAudioFile(char* pFileName) {
    AudioPlayerWnd* playerWnd = new AudioPlayerWnd(pFileName);
}

/* Handles File Manager errors. */

void IOpenDSSFileManager::onError(int cmdId, int errorCode) {
    if(cmdId == 0) {
        char msgTitle[] = "Error";
        char msgText[] = "Cannot open this directory!";
        MessageBox* pMsgBox = new MessageBox(msgTitle, msgText, 4);
        gPsGuiMan->listenKeyboard((ExtWindowCtrl*)pMsgBox);
        ((ExtWindowCtrl*)pMsgBox)->freeWnd();
        gFileManWnd->redraw();
        gPsGuiMan->listenKeyboard((ExtWindowCtrl*) gFileManWnd);
    }
}

/* Handles File Manager successed responses. */

void IOpenDSSFileManager::onResult(int cmdId, int resultCode) {
    gFileManWnd->onFileManResult(cmdId, resultCode);
    if(cmdId == 1) {
        openAudioFile(gFileManWnd->getSelectedFileName());
    }
}

/* Handles File Manager directory list. */

void IOpenDSSFileManager::onDirectoryRead(tinydir_file* files) {
    gFileManWnd->onDirectoryRead(files);
}

/* Handles keyboard pressed keys. */

void IOpenDSSPseudoGUIManager::onKeyPressed(char k, ExtWindowCtrl* pExtWnd) {
    if((int)k == 2 || (int)k == 3) {
        ListBoxCtrl* mFileListBox = ((ListBoxCtrl*)gFileManWnd->hCtrls[0]);
        if(strcmp(pExtWnd->id, "fileManWnd") == 0) {
            if(gFileManWnd->getControlsSize() > 0) {
                mFileListBox->onKeyPressed(k);
            }
        }
    } else if((int)k == 10) { // ENTER key
        if(strcmp(pExtWnd->id, "fileManWnd") == 0
            || strcmp(pExtWnd->id, "articlesListWnd") == 0) {
            if (k == 'q') {
                delete gFileManWnd;
            }
        }
    }
}

