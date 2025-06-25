#include <windows/filemanw.h>

#define MAX_FILEMAN_LENGTH 32

int scrWidth, scrHeight;

FileManagerWnd::FileManagerWnd(

) {
    hTitle = (char*)malloc(85 * sizeof(char));

    #ifdef _MSVC2005G
        sprintf_s(hTitle, 85, "File Manager");
        sprintf_s(id, 60, "fileManWnd");
    #else
        sprintf(hTitle, "File Manager");
        sprintf(id, "fileManWnd");
    #endif

    hScreen = stdscr;

    getmaxyx(hScreen, scrHeight, scrWidth);

    gWidth = scrWidth - 4;
    gHeight = scrHeight - 3;
}

void FileManagerWnd::createWnd() {
    hWnd = newwin(
        gHeight, gWidth,
        ((scrHeight - gHeight) / 2) + 1,
        ((scrWidth - gWidth) / 2)
    );

    keypad(hWnd, true);
    box(hWnd, 0, 0);
    mvwprintw(
        hWnd,
        0, (double)(gWidth - strlen(hTitle)) / 2,
        "%c %s %c",
        0xB4, hTitle, 0xC3
    );

    wrefresh(hWnd);
}

void FileManagerWnd::listenKbd() {
    hKbdListening = true;
    while(hKbdListening) {
        char key = wgetch(hWnd);
        onKeyPressed(key);
    }
}

void FileManagerWnd::readCurrentDir() {
    if(hFileMan == NULL) {
        hFileMan = new FileManager(/* */);
    }

    int result = hFileMan->readCurrentDir();

    if(result < 0) {
        mvwprintw(hWnd, 2, 2, "Epic Fail");
        mvwprintw(hWnd, 4, 2, "Path: [%s]", hFileMan->getCurrentPath());
        wrefresh(hWnd);
    } else {
        mvwprintw(hWnd, 2, 2, "Success!");
        mvwprintw(hWnd, 4, 2, "Path: [%s]", hFileMan->getCurrentPath());
        wrefresh(hWnd);
    }
}

void FileManagerWnd::onKeyPressed(char key) {
    if(key == 'q') {
        hKbdListening = false;
        return;
    }

    /*mvwprintw(
        hWnd, 2, 2, "%s",
        hFileMan->getRealPath(hFileMan->getCurrentPath())
    );*/

    wrefresh(hWnd);
}
