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

