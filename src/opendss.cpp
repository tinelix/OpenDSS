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
    gFileMan->setSelectionIndex(0);
}

/* Application main function */

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

void IOpenDSSPseudoGUIManager::onKeyPressed(char k) {
    if((int)k == 2 || (int)k == 3) {
        int index = gFileMan->getSelectionIndex();

        if((int)k == 3 && index > 0) { // top arrow key
            index--;
        } else if((int)k == 2 && index < gFileMan->getFilesCount()) { // bottom arrow key
            index++;
        }

        int list_index = index + 1;
        if(index >= 0 && index < gFileMan->getFilesCount()) {
            gPsGuiMan->drawListPointer(
                gFileManWnd,
                2,
                gFileMan->getSelectionIndex() + 1,
                false
            );
        }

        if(index < gFileMan->getFilesCount()) {
            gPsGuiMan->drawListPointer(
                        gFileManWnd,
                        2,
                        list_index,
                        true
            );
        }

        if(index <= gFileMan->getFilesCount() - 1) {
            list_index = index + 1;
                gFileMan->setSelectionIndex(index);
        }
    }

    /* Handles keyboard pressed keys except for the 'q' key
     *
     * The 'q' key is responsible for exiting the program.
     */

    if(k != 'q') {
        gPsGuiMan->listenKeyboard();
    }
}
