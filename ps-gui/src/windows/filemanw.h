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

#pragma once

#ifdef _MSVC		/* If it turns out that we are building a 
					   project using Microsoft Visual Studio. */
	#ifdef _PDCURSES
		#include <curses.h>
	#else
		#include <ncurses.h>
	#endif
#else
	#ifdef __PDCURSES__
		#include <curses.h>
	#elif __MINGW64__
		#include <ncurses/ncurses.h>
	#else
		#include <ncurses.h>
	#endif
#endif

#include <tinydir/tinydir.h>

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <controls/uictrl.h>
#include <controls/extwnd.h>
#include <controls/listbox.h>
#include <controls/msgbox.h>
#include <utils/extstr.h>
#include <utils/fileman.h>
#include <interfaces/filemani.h>

class FileManagerWnd : public ExtWindowCtrl
{
	public:
		FileManagerWnd(FileManager* pFileMan, IFileManager* pInterface, WINDOW* screen);
		char* getSelectedFileName();
		void listen(bool value);
		void onKeyPressed(char k);
		void onDirectoryRead(tinydir_file* files);
		void onFileManResult(int cmdId, int resultCode);
		void onFileManError(int cmdId, int errorCode);
		bool disableListening;
	private:
		FileManager* gFileMan;
		IFileManager* gInterface;
		char* gSelectedFileName;
};

