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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

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

#include <framedir/include/framedir.h>

#include <version.h>
#include <interfaces/pguimani.h>
#include <controls/extwnd.h>

// Default colors

#define COLOR_LIGHT_WHITE			255
#define COLOR_GRAY					240
#define COLOR_DARK_GRAY				235
#define COLOR_DARK_GREEN			 28
#define COLOR_DEEP_BLACK			 16
#define COLOR_LIGHT_RED				 82
#define COLOR_LIGHT_GREEN			 47
#define COLOR_BLUE_SKY				 68

// Colors for console with limited color palettes

#define COLOR_LIGHT_WHITE_4BIT		  6
#define COLOR_GRAY_4BIT				  7
#define COLOR_DARK_GRAY_4BIT		  8
#define COLOR_DEEP_BLACK_4BIT		 16
#define COLOR_LIGHT_RED_4BIT		  1
#define COLOR_LIGHT_GREEN_4BIT		  2
#define COLOR_BLUE_SKY_4BIT		      3
#define COLOR_LIGHT_CYAN_4BIT		  11

class PseudoGUIManager
{
	public:
		PseudoGUIManager(IPseudoGUIManager* pInterface);
		~PseudoGUIManager();
		void showTopVersionInfo();
		void listenKeyboard();
		void listenKeyboard(ExtWindowCtrl* pExtWnd);
		WINDOW* getScreen();
	private:
		IPseudoGUIManager	*gInterface;
		WINDOW				*screen;
		char                gWndTitle[255];
		int					gActiveWidth;
		int					gActiveHeight;
};

