/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------
 *  Copyright © 2024, 2025 Dmitry Tretyakov (aka. Tinelix)
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

#ifndef _OPENDSS_CONTROLS_EXTWND_H
#define _OPENDSS_CONTROLS_EXTWND_H

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

#include <controls/uictrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ExtWindowCtrl {
    public:
        ExtWindowCtrl();
        ExtWindowCtrl(char* pId);
        ExtWindowCtrl(char* pId, ExtWindowCtrl* pParent);
        ~ExtWindowCtrl();
        char hTitle[255], id[60];
        int hWidth, hHeight;
        WINDOW* hWnd;
        WINDOW* hScreen;
        UIControl** hCtrls;
        ExtWindowCtrl** hChildWnds;
        ExtWindowCtrl* gParent;
        void addControl(UIControl* pCtrl);
        void addControl(UIControl* pCtrl, int index);
        void addChildWindow(char* id, char* title, int width, int height, int x, int y);
        int getControlsSize();
        int getChildWindowsSize();
        void redraw();
        void clear();
        virtual void freeWnd();
        virtual void listen(bool value);
        virtual void onKeyPressed(char k);
        virtual void onKeyPressed(char k, char prev_k);
    private:
        int gCtrlSize;
        int gChildWndsSize;
};

#endif

