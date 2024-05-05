#include <ncurses.h>
#include <stdio.h>                  // Linking standard C functions
#include <dirent.h>

// Linking FFmpeg headers
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "utils/fileman.h"
#include "utils/pguiman.h"

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
        void onKeyPressed(char k);
};

IOpenDSSPseudoGUIManager    *gPsGUIManInterface;

PseudoGUIManager            *gPsGuiMan;
FileManager                 *gFileMan;
ExtWindow                   *gFileManWnd;

/* Creates File Manager window and shows directory listing. */

void openFileManager() {
    char wndTitle[] = "File Manager";
    gFileManWnd = gPsGuiMan->createWindow(wndTitle, 0, 0, true);
    gFileMan->readCurrentDir();
}

int main() {
    gPsGUIManInterface = new IOpenDSSPseudoGUIManager();
    gFileManInterface = new IOpenDSSFileManager();

    gFileMan = new FileManager((IFileManager*)gFileManInterface);
    gPsGuiMan = new PseudoGUIManager((IPseudoGUIManager*)gPsGUIManInterface);

    gPsGuiMan->showTopVersionInfo();
    openFileManager();
    gPsGuiMan->listenKeyboard();
    delete gPsGuiMan;
    return 0;
}

/* Handles File Manager errors. */

void IOpenDSSFileManager::onError(int cmdId, int errorCode) {
    mvwprintw((WINDOW*)gFileManWnd, 1, 1, "Oops");
}

/* Handles File Manager successed responses. */

void IOpenDSSFileManager::onResult(int cmdId, int resultCode) {

}

/* Handles File Manager directory list. */

void IOpenDSSFileManager::onDirectoryRead(struct dirent* ent, int index) {
    mvwprintw((WINDOW*)gFileManWnd, index + 1, 2, "%s", ent->d_name);
    wrefresh((WINDOW*)gFileManWnd);
}

/* Handles keyboard pressed keys. */

void IOpenDSSPseudoGUIManager::onKeyPressed(char k) {
    char objName[255];
    struct dirent *ent, *prev_ent;

    if((int)k == 2) { // bottom arrow key
        int index = gFileMan->getSelectionIndex() + 1;
        if(index > 0) {
            prev_ent = gFileMan->getFile(gFileMan->getSelectionIndex());
            gPsGuiMan->drawText(gFileManWnd, prev_ent->d_name, 2, index);
        }

        ent = gFileMan->getFile(index);
        sprintf(objName, "* %s", ent->d_name);
        gPsGuiMan->drawText(gFileManWnd, objName, 2, index + 1);
    } else if((int)k == 3 && gFileMan->getSelectionIndex() > 0) { // top arrow key
        int index = gFileMan->getSelectionIndex() - 1;
        prev_ent = gFileMan->getFile(gFileMan->getSelectionIndex());
        gPsGuiMan->drawText(gFileManWnd, prev_ent->d_name, 2, gFileMan->getSelectionIndex() + 1);

        ent = gFileMan->getFile(index);
        sprintf(objName, "* %s", ent->d_name);
        gPsGuiMan->drawText(gFileManWnd, objName, 2, index + 1);
    }
    gPsGuiMan->listenKeyboard();
}
