#include <ncurses.h>
#include <stdio.h>                  // Linking standard C functions
#include <dirent.h>

// Linking FFmpeg headers
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include <utils/fileman.h>
#include <utils/pguiman.h>
#include <controls/extwnd.h>
#include <controls/listbox.h>

#include <interfaces/fileman.h>
#include <interfaces/pguiman.h>
#include <unistd.h>

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
    gFileManWnd = gPsGuiMan->createWindow(wndTitle, 0, 0, true);
    sprintf(gFileManWnd->id, "fm_wnd");
    gFileMan->readCurrentDir();

    ListBoxCtrl *mFileListBox = new ListBoxCtrl(gFileManWnd, (int)gFileMan->getFilesCount());
    mFileListBox->setSelectionIndex(0);
    gFileManWnd->addControl((UIControl*)mFileListBox);
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
    mvwprintw(gFileManWnd->hWnd, 1, 1, "Oops");
}

/* Handles File Manager successed responses. */

void IOpenDSSFileManager::onResult(int cmdId, int resultCode) {

}

/* Handles File Manager directory list. */

void IOpenDSSFileManager::onDirectoryRead(struct dirent* ent, int index) {
    mvwprintw(gFileManWnd->hWnd, index + 1, 4, "%s", ent->d_name);
    wrefresh(gFileManWnd->hWnd);
}

/* Handles keyboard pressed keys. */

void IOpenDSSPseudoGUIManager::onKeyPressed(char k, ExtWindowCtrl* pExtWnd) {
    if((int)k == 2 || (int)k == 3) {
        if(strcmp(pExtWnd->id, "fm_wnd") == 0) {
            if(gFileManWnd->getControlsSize() > 0) {
                ((ListBoxCtrl*)gFileManWnd->hCtrls[0])->onKeyPressed(k);
            }
        }
    } else if((int)k == 10) { // ENTER key
        // TODO: add 'Open File' function implementation
    }

    if(k != 'q') {
        gPsGuiMan->listenKeyboard(gFileManWnd);
    } else {
        delete gFileManWnd;
    }
}
