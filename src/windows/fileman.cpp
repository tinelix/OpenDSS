#include "fileman.h"

#define MAX_FILENAME_LENGTH 32

FileManagerWnd::FileManagerWnd(FileManager *pFileMan, IFileManager *pInterface) {
    sprintf(hTitle, "File Manager");
    sprintf(id, "fileManWnd");

    gFileMan = pFileMan;
    gInterface = pInterface;

    int gActiveWidth, gActiveHeight;

    getmaxyx(stdscr, gActiveHeight, gActiveWidth);

    hWidth = gActiveWidth - 2;
    hHeight = gActiveHeight - 3;

    hWnd = newwin(hHeight, hWidth,
                  ((gActiveHeight - hHeight) / 2) + 1,
                  (gActiveWidth - hWidth) / 2);

    keypad(hWnd, true);

    box(hWnd, 0, 0);                            // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        hWnd,
        0, (gActiveWidth - wcslen((wchar_t*)hTitle)) / 2.5,
        "\u2524 %s \u251c", hTitle
    );

    wbkgd(hWnd, COLOR_PAIR(2));

    wrefresh(hWnd);

    disableListening = false;
}

char* FileManagerWnd::getSelectedFileName() {
    return gSelectedFileName;
}

void FileManagerWnd::onKeyPressed(char k) {

    if(k == 'q') {
        disableListening = false;
        return;
    }

    ListBoxCtrl* mFileListBox = ((ListBoxCtrl*)hCtrls[0]);
    char fname[255];
    dirent* ent = gFileMan->getFile(
        mFileListBox->getSelectionIndex()
    );
    sprintf(fname, "%s/%s", gFileMan->getCurrentPath(), ent->d_name);
    if(k == (int)10) {
        #ifdef __MINGW64__
            struct stat s;
            stat(fname, &s);
            if (s.st_mode & S_IFDIR) {
                gFileMan->readDir(fname);
            } else {
                if(ExtString::strendq(fname, ".json")) {
                    char msgTitle[] = "Opening file";
                    MessageBox* pMsgBox = new MessageBox(msgTitle, fname, 5);
                    ((ExtWindowCtrl*)pMsgBox)->freeWnd();
                    redraw();
                    gSelectedFileName = fname;
                    disableListening = true;
                    gInterface->onResult(1, 0);
                }
            }
        #else
            if(ent->d_type == 4) { // if it's directory
                gFileMan->readDir(fname);
            } else {
                if(ExtString::strendq(fname, ".mp3")) {
                    char msgTitle[] = "Opening file";
                    MessageBox* pMsgBox = new MessageBox(msgTitle, fname, 5);
                    ((ExtWindowCtrl*)pMsgBox)->freeWnd();
                    redraw();
                    gSelectedFileName = fname;
                    disableListening = true;
                    gInterface->onResult(1, 0);
                }
            }
        #endif
    } else {
        mFileListBox->onKeyPressed(k);
    }

    if(!disableListening) {
        k = wgetch(hWnd);
        onKeyPressed(k);
    }
}

void FileManagerWnd::onDirectoryRead(dirent** ents) {
    ListBoxCtrl *mFileListBox;
    if(!hCtrls[0]) {
        mFileListBox = new ListBoxCtrl(this, gFileMan->getFilesCount(), true);
        mFileListBox->setSelectionIndex(0);
        mFileListBox->hY = 4;
        mFileListBox->hX = 2;
        mFileListBox->hHeight = hHeight - 6;
        mFileListBox->hWidth = hWidth - 4;

        addControl((UIControl*)mFileListBox);
    } else {
        mFileListBox = ((ListBoxCtrl*) hCtrls[0]);
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
            AudioTager *pTagger = new AudioTager();
            AudioTags *data = pTagger->readTags(full_fname);
            if(data) {
                mvwprintw(
                        hWnd,
                        mFileListBox->hY + i,
                        MAX_FILENAME_LENGTH + 19,
                        "%s - %s",
                        data->artist, data->title
                );
            }
        }
        mFileListBox->addListItem(i, item);
    }

    wrefresh(hWnd);

    char k = wgetch(hWnd);

    onKeyPressed(k);
}

void FileManagerWnd::onFileManResult(int cmdId, int resultCode) {
    if(cmdId == 0) {
        clear();
        mvwprintw(hWnd, 2, 2, "%s", gFileMan->getCurrentPath());
    }
}

void FileManagerWnd::onFileManError(int cmdId, int errorCode) {

}
