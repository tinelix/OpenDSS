/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU General Public License 3 (or any later version) and/or Apache License 2
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */

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

    tinydir_file file = gFileMan->getFile(
        mFileListBox->getSelectionIndex()
    );

    sprintf(fname, "%s/%s", gFileMan->getRealPath(gFileMan->getCurrentPath()), file.name);
    if(k == (int)10) {
        if(file.is_dir) { // if it's directory
            gFileMan->readDir(fname);
        } else if(ExtString::strendq(fname, ".mp3")) {
            char msgTitle[] = "Opening file";
            MessageBoxU* pMsgBox = new MessageBoxU(msgTitle, fname, 5);
            ((ExtWindowCtrl*)pMsgBox)->freeWnd();
            redraw();
            gSelectedFileName = fname;
            disableListening = true;
            gInterface->onResult(1, 0);
        }
    } else {
        mFileListBox->onKeyPressed(k);
    }

    if(!disableListening) {
        k = wgetch(hWnd);
        onKeyPressed(k);
    }
}

void FileManagerWnd::onDirectoryRead(tinydir_file* files) {
    ListBoxCtrl *mFileListBox;

    for(int i = 0; i < hWidth - 4; i++) {
        mvwaddch(hWnd, 2, i + 2, ' ');
    }

    mvwprintw(hWnd, 2, 2, "%s", gFileMan->getCurrentPath());
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
        sprintf(item->title, "%s", files[i].name);
        if(strlen(item->title) > MAX_FILENAME_LENGTH) {
            ExtString::strcut(item->title, MAX_FILENAME_LENGTH - 3, -1);
            sprintf(item->title + MAX_FILENAME_LENGTH - 3, "...");
        }
        if(i <= mFileListBox->hHeight
            && ExtString::strendq(files[i].name, ".mp3"))  {
            char full_fname[600];
            sprintf(full_fname, "%s/%s", gFileMan->getCurrentPath(), files[i].name);
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
