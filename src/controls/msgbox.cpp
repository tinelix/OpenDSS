#include "msgbox.h"

MessageBox::MessageBox(char *pWndTitle, char *pMsgText) {
    sprintf(id, "msgBoxWnd");
    sprintf(hTitle, "%s", pWndTitle);
    sprintf(hMsgText, "%s", pMsgText);

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

MessageBox::MessageBox(char *pWndTitle, char *pMsgText, int pBgColor) {
    sprintf(id, "msgBoxWnd");
    sprintf(hTitle, "%s", pWndTitle);
    sprintf(hMsgText, "%s", pMsgText);

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

void MessageBox::drawMessageText() {

}
