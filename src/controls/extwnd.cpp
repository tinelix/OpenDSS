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

#include "extwnd.h"

ExtWindowCtrl::ExtWindowCtrl() {
    hWidth = 8;
    hHeight = 8;
    gCtrlSize = 0;
    gChildWndsSize = 0;
    hCtrls = (UIControl**)malloc(sizeof(UIControl) * 255);
    hChildWnds = (ExtWindowCtrl**)malloc(sizeof(ExtWindowCtrl) * 16);
}

ExtWindowCtrl::ExtWindowCtrl(char* pId) {
    sprintf(id, "%s", pId);
    hWidth = 8;
    hHeight = 8;
    gCtrlSize = 0;
    gChildWndsSize = 0;
    hCtrls = (UIControl**)malloc(sizeof(UIControl) * 255);
    hChildWnds = (ExtWindowCtrl**)malloc(sizeof(ExtWindowCtrl) * 16);
}

ExtWindowCtrl::~ExtWindowCtrl() {
    free(hCtrls);
    free(hChildWnds);
}

void ExtWindowCtrl::addControl(UIControl* pCtrl) {
    hCtrls[gCtrlSize] = pCtrl;
    gCtrlSize++;
}

void ExtWindowCtrl::addChildWindow(char* id, char* title, int width, int height, int x, int y) {

    ExtWindowCtrl *pExtWnd = new ExtWindowCtrl(id);

    int realWidth = 5;
    int realHeight = 5;

    realWidth = width;
    if(width <= 5) {
        realWidth = 5;
    } else {
        realWidth = width;
    }

    if(height <= 5) {
        realHeight = 5;
    } else {
        realHeight = height;
    }

    pExtWnd->hWnd = newwin(realHeight, realWidth, y + 1, x + 1);

    sprintf(pExtWnd->hTitle, "%s", title);      // <-- store window text in ExtWindow object

    pExtWnd->hWidth = realWidth;
    pExtWnd->hHeight = realHeight;

    box(pExtWnd->hWnd, 0, 0);                   // <-- draw window borders

    mvwprintw(                                  // <-- draw window text in top border area
        pExtWnd->hWnd,
        0, (pExtWnd->hWidth - strlen(pExtWnd->hTitle) - 4) / 2,
        "\u2524 %s \u251c", pExtWnd->hTitle
    );

    wbkgd(pExtWnd->hWnd, COLOR_PAIR(2));

    keypad(pExtWnd->hWnd, true);
    wrefresh(pExtWnd->hWnd);

    hChildWnds[gChildWndsSize] = pExtWnd;

    gChildWndsSize++;
}

int ExtWindowCtrl::getControlsSize() {
    return gCtrlSize;
}

int ExtWindowCtrl::getChildWindowsSize() {
    return gChildWndsSize;
}

void ExtWindowCtrl::redraw() {
    touchwin(hWnd);
    wrefresh(hWnd);
}

void ExtWindowCtrl::clear() {
    wclear(hWnd);
    box(hWnd, 0, 0);
    mvwprintw(                                  // <-- draw window text in top border area
        hWnd,
        0, (hWidth - strlen(hTitle) - 4) / 2,
        "\u2524 %s \u251c", hTitle
    );
}

void ExtWindowCtrl::freeWnd() {
    wclear(hWnd);
    wrefresh(hWnd);
    delwin(hWnd);
    refresh();
}

