#include <opendss.h>
#include <utils/tuiman.h>
#include <windows/filemanw.h>

TUIManager*     gTuiMan;

int main() {
    FileManagerWnd* gFileManWnd;

    gTuiMan = new TUIManager();
    gTuiMan->showTopBanner();

    gFileManWnd = new FileManagerWnd();
    gFileManWnd->createWnd();

    gFileManWnd->readCurrentDir();

    gFileManWnd->listenKbd();

    return 0;
}
