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

#ifndef OPENDSS_UTILS_PGUIMAN_H
#define OPENDSS_UTILS_PGUIMAN_H

#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#else
    #include <ncurses.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../interfaces/pguiman.h"
#include "../controls/extwnd.h"
#include <locale.h>
#include "../version.h"

#define COLOR_LIGHT_WHITE       255
#define COLOR_GRAY              240
#define COLOR_DARK_GRAY         235
#define COLOR_DARK_GREEN         28
#define COLOR_DEEP_BLACK         16
#define COLOR_LIGHT_RED          82
#define COLOR_LIGHT_GREEN        47
#define COLOR_BLUE_SKY           68

class PseudoGUIManager {
    public:
        PseudoGUIManager(IPseudoGUIManager *pInterface);
        ~PseudoGUIManager();
        void showTopVersionInfo();
        void listenKeyboard();
        void listenKeyboard(ExtWindowCtrl *pExtWnd);
        ExtWindowCtrl* createWindow(char* id, char* title, int width, int height, bool alignCenter);
        void clearWindow(ExtWindowCtrl* pExtWnd);
        void drawText(ExtWindowCtrl *wnd, char* text, int x, int y);
        int gActiveWidth, gActiveHeight;
    private:
        IPseudoGUIManager   *gInterface;
        ExtWindowCtrl       *gWnd;
        char                gWndTitle[255];
};

#endif // OPENDSS_UTILS_PGUIMAN_H
