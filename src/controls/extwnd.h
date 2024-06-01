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

#ifndef OPENDSS_CONTROLS_EXTWND_H
#define OPENDSS_CONTROLS_EXTWND_H

#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#else
    #include <ncurses.h>
#endif

#include "uictrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ExtWindowCtrl {
    public:
        ExtWindowCtrl();
        ExtWindowCtrl(char* pId);
        ~ExtWindowCtrl();
        char hTitle[255], id[60];
        int hWidth, hHeight;
        WINDOW* hWnd;
        UIControl** hCtrls;
        ExtWindowCtrl** hChildWnds;
        void addControl(UIControl* pCtrl);
        void addChildWindow(char* id, char* title, int width, int height, int x, int y);
        int getControlsSize();
        int getChildWindowsSize();
        void redraw();
        void clear();
        void freeWnd();
    private:
        int gCtrlSize;
        int gChildWndsSize;
};

#endif
