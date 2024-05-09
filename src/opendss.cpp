#include <ncurses.h>
#include <stdio.h>                  // Linking standard C functions
#include <dirent.h>
#include <unistd.h>

// Linking FFmpeg headers
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "utils/fileman.h"
#include "utils/pguiman.h"
#include "utils/extstr.h"

#include "controls/extwnd.h"
#include "controls/listbox.h"

#include "interfaces/fileman.h"
#include "interfaces/pguiman.h"

#define MAX_FILENAME_LENGTH 32

class IOpenDSSFileManager : IFileManager {
    public:
        void onError(int cmdId, int errorCode);
        void onResult(int cmdId, int resultCode);
        void onDirectoryRead(dirent** ents);
};

IOpenDSSFileManager         *gFileManInterface;

class IOpenDSSPseudoGUIManager : IPseudoGUIManager {
    public:
        void onKeyPressed(char k) {};
        void onKeyPressed(char k, ExtWindowCtrl *pExtWnd);
};

IOpenDSSPseudoGUIManager    *gPsGUIManInterface;

PseudoGUIManager            *gPsGuiMan;
FileManager                 *gFileMan;
ExtWindowCtrl               *gFileManWnd;
//AudioTager                  *gAudioTager;

/* Creates File Manager window and shows directory listing. */

void openFileManager() {
    char wndTitle[] = "File Manager";
    gFileManWnd = gPsGuiMan->createWindow(wndTitle, -2, -2, true);
    sprintf(gFileManWnd->id, "fm_wnd");
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

    gPsGuiMan->listenKeyboard(gFileManWnd);
    delete gPsGuiMan;
    return 0;
}

/* Handles File Manager errors. */

void IOpenDSSFileManager::onError(int cmdId, int errorCode) {
    char errorStr[255];
    sprintf(errorStr, "%s - Oops! Error code: %d", gFileMan->getCurrentPath(), errorCode);
    gPsGuiMan->drawText(gFileManWnd, errorStr, 2, 2);
}

/* Handles File Manager successed responses. */

void IOpenDSSFileManager::onResult(int cmdId, int resultCode) {
    if(cmdId == 0) {
        gPsGuiMan->clearWindow(gFileManWnd);
        gPsGuiMan->drawText(gFileManWnd, gFileMan->getCurrentPath(), 2, 2);
    }
}

/* Handles File Manager directory list. */

void IOpenDSSFileManager::onDirectoryRead(dirent** ents) {
    ListBoxCtrl *mFileListBox;
    if(!gFileManWnd->hCtrls[0]) {
        mFileListBox = new ListBoxCtrl(gFileManWnd, gFileMan->getFilesCount());
        mFileListBox->setSelectionIndex(0);
        mFileListBox->hY = 4;
        mFileListBox->hX = 2;
        mFileListBox->hHeight = gFileManWnd->hHeight - 6;

        gFileManWnd->addControl((UIControl*)mFileListBox);
    } else {
        mFileListBox = ((ListBoxCtrl*) gFileManWnd->hCtrls[0]);
        mFileListBox->recreate(gFileMan->getFilesCount());
    }

    for(int i = 0; i < gFileMan->getFilesCount(); i++) {
        ListItem* item = new ListItem();
        sprintf(item->title, "%s", ents[i]->d_name);
        if(strlen(item->title) > MAX_FILENAME_LENGTH) {
            ExtString::strcut(item->title, MAX_FILENAME_LENGTH - 3, -1);
            sprintf(item->title + MAX_FILENAME_LENGTH - 3, "...");
        }
        if(i <= mFileListBox->hHeight
            && ExtString::strendq(ents[i]->d_name, ".mp3"))  {
            char full_fname[600];
            sprintf(full_fname, "%s/%s", gFileMan->getCurrentPath(), ents[i]->d_name);
            mvwprintw(
                gFileManWnd->hWnd,
                i + mFileListBox->hY,
                MAX_FILENAME_LENGTH,
                "0:00:00.000"
            );
        }
        mFileListBox->addListItem(i, item);
    }

    wrefresh(gFileManWnd->hWnd);
}

/* Handles keyboard pressed keys. */

void IOpenDSSPseudoGUIManager::onKeyPressed(char k, ExtWindowCtrl* pExtWnd) {
    if((int)k == 2 || (int)k == 3) {
        ListBoxCtrl* mFileListBox = ((ListBoxCtrl*)gFileManWnd->hCtrls[0]);
        if(strcmp(pExtWnd->id, "fm_wnd") == 0) {
            if(gFileManWnd->getControlsSize() > 0) {
                mFileListBox->onKeyPressed(k);
            }
        }
    } else if((int)k == 10) { // ENTER key
        ListBoxCtrl* mFileListBox = ((ListBoxCtrl*)gFileManWnd->hCtrls[0]);
        dirent* ent = gFileMan->getFile(
            mFileListBox->getSelectionIndex()
        );
        char fname[255];
        sprintf(fname, "%s/%s", gFileMan->getCurrentPath(), ent->d_name);
        if(ent->d_type == 4) { // if it's directory
            gFileMan->readDir(fname);
        }
    }

    if(k != 'q') {
        gPsGuiMan->listenKeyboard(gFileManWnd);
    } else {
        delete gFileManWnd;
    }
}

