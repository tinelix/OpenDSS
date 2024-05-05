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
WINDOW                      *gFileManWnd;

int main() {
    gPsGUIManInterface = new IOpenDSSPseudoGUIManager();
    gFileManInterface = new IOpenDSSFileManager();

    gFileMan = new FileManager((IFileManager*)gFileManInterface);
    gPsGuiMan = new PseudoGUIManager((IPseudoGUIManager*)gPsGUIManInterface);

    gPsGuiMan->showTopVersionInfo();
    gPsGuiMan->listenKeyboard();
    delete gPsGuiMan;
    return 0;
}

void IOpenDSSFileManager::onError(int cmdId, int errorCode) {
    mvwprintw(gFileManWnd, 1, 1, "Oops");
}

void IOpenDSSFileManager::onResult(int cmdId, int resultCode) {

}

void IOpenDSSFileManager::onDirectoryRead(struct dirent* ent, int index) {
    mvwprintw(gFileManWnd, index + 1, 1, "%s", ent->d_name);
}

void IOpenDSSPseudoGUIManager::onKeyPressed(char k) {
    if(k == 'f') {
        char wndTitle[] = "File Manager";
        gFileManWnd = gPsGuiMan->createWindow(wndTitle, 0, 0, true);
        gFileMan->readCurrentDir();
    }
    gPsGuiMan->listenKeyboard();
}
