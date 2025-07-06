#include <windows/filemanw.h>

#define MAX_FILEMAN_LENGTH 32

int scrWidth, scrHeight;

FileManagerWnd::FileManagerWnd() {
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

WINDOW* FileManagerWnd::getWnd() {
    return hWnd;
}

void FileManagerWnd::prepare() {
    if(hFileMan == NULL || hInterface == NULL) {
        hInterface = new IFileSelector();
        hInterface->setFileManager(this, hFileMan);
        hFileMan = new FileManager(hInterface);
    }
}

void FileManagerWnd::listenKbd() {
    hKbdListening = true;
    while(hKbdListening) {
        char key = wgetch(hWnd);
        onKeyPressed(key);
    }
}

void FileManagerWnd::readCurrentDir() {
    hFileMan->readCurrentDir();
}

void FileManagerWnd::onKeyPressed(char key) {
    if(key == 'q') {
        hKbdListening = false;
        return;
    }

    wrefresh(hWnd);
}

void FileManagerWnd::onFileSelectorSuccess() {
    return;
}

void IFileSelector::setFileManager(
    ExtWindowCtrl* pWindow,
    FileManager* pFileMan
) {
    hWindow     = pWindow;
    hFileMan    = pFileMan;
}

void IFileSelector::onError(char* path, int cmdId, int errorCode) {
    if(cmdId == 0) {
        char msgTitle[] = "Error";
        char msgText[480];

        #ifdef _MSVC2005G
            sprintf_s(
                msgText, 256,
        #else
            sprintf(
                msgText,
        #endif
                "Cannot open this directory! Path: %s",
                path
            );
    }
}

void IFileSelector::onResult(char* path, int cmdId, int resultCode) {
    return;
}

void IFileSelector::onDirectoryRead(
    char* path, framedir_file* files
) {
    WINDOW* hWnd;

    if(
        hWindow == NULL ||
        strcmp(hWindow->id, "fileManWnd") == 0
    )
        return;

    FileManagerWnd* hFileManWnd = (FileManagerWnd*)hWindow;

    hWnd = hWindow->getWnd();
    mvwprintw(hWnd, 2, 2, "%s", path);
    wrefresh(hWnd);


}
