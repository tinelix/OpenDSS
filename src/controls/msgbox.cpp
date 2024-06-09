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

#include "msgbox.h"

MessageBoxU::MessageBoxU(char* pWndTitle, char* pMsgText) {
    #ifdef _MSVC
        sprintf_s(id, "msgBoxWnd");
        sprintf_s(hTitle, 75, "%s", pWndTitle);
        sprintf_s(hMsgText, 255, "%s", pMsgText);
    #else    
        sprintf(id, "msgBoxWnd");
        sprintf(hTitle, "%s", pWndTitle);
        sprintf(hMsgText, "%s", pMsgText);
    #endif

    hWidth = strlen(pMsgText) + 4;
    hHeight = 5;

    int gActiveWidth, gActiveHeight;

    getmaxyx(stdscr, gActiveHeight, gActiveWidth);

    hWnd = newwin(hHeight, hWidth,
        ((gActiveHeight - hHeight) / 2) + 1,
        (gActiveWidth - hWidth) / 2);

    keypad(hWnd, true);

    box(hWnd, 0, 0);                            // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        hWnd,
        0, (hWidth - strlen(hTitle) - 4) / 2,
        "\u2524 %s \u251c", hTitle
    );

    mvwprintw(                                  // <-- draw message text
        hWnd, 2, 2, "%s", hMsgText
    );

    wbkgd(hWnd, COLOR_PAIR(4));

    wrefresh(hWnd);
}

MessageBoxU::MessageBoxU(char* pWndTitle, char* pMsgText, int pBgColor) {
    #ifdef _MSVC
        sprintf_s(id, "msgBoxWnd");
        sprintf_s(hTitle, 75, "%s", pWndTitle);
        sprintf_s(hMsgText, 255, "%s", pMsgText);
    #else    
        sprintf(id, "msgBoxWnd");
        sprintf(hTitle, "%s", pWndTitle);
        sprintf(hMsgText, "%s", pMsgText);
    #endif

    hWidth = strlen(pMsgText) + 4;
    hHeight = 5;

    int gActiveWidth, gActiveHeight;

    getmaxyx(stdscr, gActiveHeight, gActiveWidth);

    hWnd = newwin(hHeight, hWidth, ((gActiveHeight - hHeight) / 2) + 1, (gActiveWidth - hWidth) / 2);

    keypad(hWnd, true);

    box(hWnd, 0, 0);                            // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        hWnd,
        0, (hWidth - strlen(hTitle) - 4) / 2,
        "\u2524 %s \u251c", hTitle
    );

    mvwprintw(                                  // <-- draw message text
        hWnd, 2, 2, "%s", hMsgText
    );

    wbkgd(hWnd, COLOR_PAIR(pBgColor));

    wrefresh(hWnd);
}

void MessageBoxU::drawMessageText() {

}
