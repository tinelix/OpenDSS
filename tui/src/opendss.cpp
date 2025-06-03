/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This file is part of Tinelix OpenDSS.
 *
 *  Tinelix OpenDSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Tinelix OpenDSS is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Tinelix OpenDSS. If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef _MSVC		/* If it turns out that we are building a 
					   project using Microsoft Visual Studio. */
	#ifdef _PDCURSES
		#include <curses.h>
	#else
		#include <ncurses.h>
	#endif

	#ifdef _TINYDIR
		#include <tinydir/tinydir.h>
	#else
		#include <framedir/include/framedir.h>
	#endif
#else
	#ifdef __PDCURSES__
		#include <curses.h>
	#elif __MINGW64__
		#include <ncurses/ncurses.h>
	#else
		#include <ncurses.h>
	#endif

	#ifdef _TINYDIR
		#include <tinydir/tinydir.h>
	#else
		#include <framedir/include/framedir.h>
	#endif
#endif

#include <interfaces/pguimani.h>
#include <interfaces/filemani.h>
#include <controls/extwnd.h>
#include <controls/uictrl.h>
#include <windows/filemanw.h>
#include <windows/playerw.h>
#include <utils/pguiman.h>
#include <utils/fileman.h>

PseudoGUIManager*   gPsGuiMan;
FileManager*        gFileMan;

FileManagerWnd*     gFileManWnd;
AudioPlayerWnd*     gPlayerWnd;

class IOpenDSSFileManager : IFileManager {
public:
	void onError(int cmdId, int errorCode);
	void onResult(int cmdId, int resultCode);
	void onDirectoryRead(framedir_file* files);
};

IOpenDSSFileManager* gFileManInterface;

class IOpenDSSPseudoGUIManager : IPseudoGUIManager {
public:
	void onKeyPressed(char k) {};
	void onKeyPressed(char k, ExtWindowCtrl* pExtWnd);
};

IOpenDSSPseudoGUIManager* gPsGUIManInterface;

void openFileManager();
void openAudioFile(char* pFileName);

int main() {
    gPsGUIManInterface = new IOpenDSSPseudoGUIManager();
    gFileManInterface = new IOpenDSSFileManager();

    gFileMan = new FileManager((IFileManager*)gFileManInterface);
    gPsGuiMan = new PseudoGUIManager((IPseudoGUIManager*)gPsGUIManInterface);

    gPsGuiMan->showTopVersionInfo();

    openFileManager();

    delete gPsGuiMan;
    return 0;
}

void openFileManager() {
    gFileManWnd = new FileManagerWnd(
        gFileMan, (IFileManager*)gFileManInterface,
        gPsGuiMan->getScreen()
    );
	gFileManWnd->openCurrentDir();
	gFileManWnd->listenKeyboard();
}

void openAudioFile(char* pFileName) {
    gPlayerWnd = new AudioPlayerWnd(
        pFileName, gFileManWnd, gPsGuiMan->getScreen()
    );
}

/* Handles File Manager errors. */

void IOpenDSSFileManager::onError(int cmdId, int errorCode) {
    if (cmdId == 0) {
        char msgTitle[] = "Error";
        char msgText[480];
        #ifdef _MSVC2005G
            sprintf_s(
                msgText, 256,
                "Cannot open this directory! Path: %s",
                gFileMan->getCurrentPath()
            );
        #else
            sprintf(
                msgText, 
                "Cannot open this directory! Path: %s",
                gFileMan->getCurrentPath()
            );
        #endif
        MessageBoxU* pMsgBox = new MessageBoxU(
            msgTitle, msgText, 4, gPsGuiMan->getScreen()
        );
        gPsGuiMan->listenKeyboard((ExtWindowCtrl*)pMsgBox);
        ((ExtWindowCtrl*)pMsgBox)->freeWnd();
        gFileManWnd->redraw();
        gPsGuiMan->listenKeyboard((ExtWindowCtrl*)gFileManWnd);
    }
}

/* Handles File Manager successed responses. */

void IOpenDSSFileManager::onResult(int cmdId, int resultCode) {
    gFileManWnd->onFileManResult(cmdId, resultCode);
    if (cmdId == 1) {
        openAudioFile(gFileManWnd->getSelectedFileName());
    }
}

/* Handles File Manager directory list. */

void IOpenDSSFileManager::onDirectoryRead(framedir_file* files) {
    gFileManWnd->onDirectoryRead(files);
}

/* Handles keyboard pressed keys. */

void IOpenDSSPseudoGUIManager::onKeyPressed(char k, ExtWindowCtrl* pExtWnd) {
    if ((int)k == 2 || (int)k == 3) {
        ListBoxCtrl* mFileListBox = ((ListBoxCtrl*)gFileManWnd->hCtrls[0]);
        if (strcmp(pExtWnd->id, "fileManWnd") == 0) {
            if (gFileManWnd->getControlsSize() > 0) {
                mFileListBox->onKeyPressed(k);
            }
        }
    }
    else if ((int)k == 10) { // ENTER key
        if (strcmp(pExtWnd->id, "fileManWnd") == 0
            || strcmp(pExtWnd->id, "articlesListWnd") == 0) {
            if (k == 'q') {
                delete gFileManWnd;
            }
        }
    }
}
