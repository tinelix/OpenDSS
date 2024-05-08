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

class IOpenDSSFileManager : IFileManager {
    public:
        void onError(int cmdId, int errorCode);
        void onResult(int cmdId, int resultCode);
        void onDirectoryRead(struct dirent* ent, int index);
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

/* Creates File Manager window and shows directory listing. */

void openFileManager() {
    char wndTitle[] = "File Manager";
    gFileManWnd = gPsGuiMan->createWindow(wndTitle, -2, -2, true);
    sprintf(gFileManWnd->id, "fm_wnd");
    ListBoxCtrl *mFileListBox = new ListBoxCtrl(gFileManWnd, (int)gFileMan->getFilesCount());
    mFileListBox->setSelectionIndex(0);
    mFileListBox->hY = 4;
    mFileListBox->hX = 2;
    mFileListBox->hHeight = gFileManWnd->hHeight - 6;
    gFileManWnd->addControl((UIControl*)mFileListBox);
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

void IOpenDSSFileManager::onDirectoryRead(struct dirent* ent, int index) {
    char short_fname[255];
    ListBoxCtrl* mFileListBox = ((ListBoxCtrl*)gFileManWnd->hCtrls[0]);
    int hY = mFileListBox->hY;
    if(index <= mFileListBox->hHeight)  {
        sprintf(short_fname, "%s", ent->d_name);
        ExtString::strcut(short_fname, 32, -1);
        mvwprintw(gFileManWnd->hWnd, index + hY, 4, "%s", short_fname);
        if(ExtString::strendq(ent->d_name, ".mp3")) {
            mvwprintw(gFileManWnd->hWnd, index + hY, 38, "0:00:00.000");
        }
    }
    wrefresh(gFileManWnd->hWnd);
}

/* Handles keyboard pressed keys. */

void IOpenDSSPseudoGUIManager::onKeyPressed(char k, ExtWindowCtrl* pExtWnd) {
    if((int)k == 2 || (int)k == 3) {
        ListBoxCtrl* mFileListBox = ((ListBoxCtrl*)gFileManWnd->hCtrls[0]);
        mFileListBox->setItemCount(gFileMan->getFilesCount());
        if(strcmp(pExtWnd->id, "fm_wnd") == 0) {
            if(gFileManWnd->getControlsSize() > 0) {
                ((ListBoxCtrl*)gFileManWnd->hCtrls[0])->onKeyPressed(k);
            }
        }
    } else if((int)k == 10) { // ENTER key
        dirent* ent = gFileMan->getFile(
            ((ListBoxCtrl*)gFileManWnd->hCtrls[0])->getSelectionIndex()
        );
        char fname[255];
        sprintf(fname, "%s/%s", gFileMan->getCurrentPath(), ent->d_name);
        if(ent->d_type == 4) { // if it's directory
            gFileMan->readDir(fname);
            ((ListBoxCtrl*)gFileManWnd->hCtrls[0])->setItemCount((int)gFileMan->getFilesCount());
        }
    }

    if(k != 'q') {
        gPsGuiMan->listenKeyboard(gFileManWnd);
    } else {
        delete gFileManWnd;
    }
}

