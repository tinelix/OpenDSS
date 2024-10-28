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

#include "msgbox.h"

MessageBoxU::MessageBoxU(char* pWndTitle, char* pMsgText, WINDOW* screen) {
    #ifdef _MSVC2005G
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

    getmaxyx(screen, gActiveHeight, gActiveWidth);

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

MessageBoxU::MessageBoxU(char* pWndTitle, char* pMsgText, int pBgColor, WINDOW* screen) {
    #ifdef _MSVC2005G
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

    getmaxyx(screen, gActiveHeight, gActiveWidth);

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
