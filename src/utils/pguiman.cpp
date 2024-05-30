#include "pguiman.h"

char key;

/* Initializes ncurses screen. */

PseudoGUIManager::PseudoGUIManager(IPseudoGUIManager *interface) {
    setlocale(LC_ALL, "");                      /* <-- set locale for correct non-ASCII characters
                                                       displaying */
    initscr();                                  /* <-- temporally clearing command prompt and initializes
                                                       empty screen of ncurses */
    keypad(stdscr, true);                       // <-- enables arrow pressed keys handling

    noecho();                                   /* <-- disables escaping of characters typed on the
                                                       keyboard                                     */
    curs_set(0);                                // <-- makes the terminal cursor invisible

    // Creates a color that will be used later if its output is supported by the terminal.

    if(has_colors()) {
        start_color();

        init_color(COLOR_LIGHT_WHITE, 768, 768, 768);
        init_color(COLOR_BLUE, 0, 142, 768);  //  <-- create RGB value for COLOR_BLUE variable
        init_pair(1, COLOR_LIGHT_WHITE, COLOR_BLUE);
        init_color(COLOR_GRAY, 384, 384, 384);  //  <-- create RGB value for COLOR_GRAY variable
        init_pair(2, COLOR_LIGHT_WHITE, COLOR_GRAY);
        init_color(COLOR_DEEP_BLACK, 0, 0, 0);  //  <-- create RGB value for COLOR_DEEP_BLACK variable
        init_pair(3, COLOR_LIGHT_WHITE, COLOR_DEEP_BLACK);
        init_color(COLOR_RED, 255, 0, 0);  //  <-- create RGB value for COLOR_RED variable
        init_pair(4, COLOR_LIGHT_WHITE, COLOR_RED);
        init_color(COLOR_DARK_GREEN, 0, 255, 0);  //  <-- create RGB value for COLOR_GREEN variable
        init_pair(5, COLOR_LIGHT_WHITE, COLOR_DARK_GREEN);
        init_pair(6, COLOR_LIGHT_GREEN, COLOR_GRAY);
        init_pair(7, COLOR_LIGHT_GREEN, COLOR_DEEP_BLACK);
        init_color(COLOR_LIGHT_RED, 768, 120, 120);
        init_pair(8, COLOR_LIGHT_RED, COLOR_DEEP_BLACK);
        init_color(COLOR_DARK_GRAY, 255, 255, 255);
        init_pair(9, COLOR_DARK_GRAY, COLOR_GRAY);
        init_pair(10, COLOR_DARK_GRAY, COLOR_DEEP_BLACK);
    }

    bkgd(COLOR_PAIR(3));

    gInterface = interface;

    getmaxyx(stdscr, gActiveHeight, gActiveWidth);
}

/* Shows version info and copyright in console top area. */

void PseudoGUIManager::showTopVersionInfo() {

    char verInfoStr[] = "Tinelix OpenDSS v. 0.0.1. Copyright (C) 2024 Dmitry Tretyakov\n";

    /*      int int
     * move( y , x )                                                <-- moves cursor
     *
     *              const char*
     * printw("String [%s/%d/%f/%c]", [args])                       <-- print text in screen
     *
     *         int int   int     attr_t    short   const void *
     * mvchgat( y , x, length, attributes, color,    options   )    <-- set text attributes or colors
     *                                                                  attrubutes: A_BLANK, A_BOLD,
     *                                                                              A_NORMAL
     *                                                                  color: number from init_pair(),
     *                                                                         argument #0
     */

    move(0, (gActiveWidth - strlen(verInfoStr)) / 2);
    printw("%s", verInfoStr);
    mvchgat(0, 0, -1, A_NORMAL, 1, NULL);  // sets the background color for a specific line

    refresh();
}

/* Listens pressed keys on keyboard. */

void PseudoGUIManager::listenKeyboard() {
    key = getch();
    gInterface->onKeyPressed(key);
}

void PseudoGUIManager::listenKeyboard(ExtWindowCtrl* hWndCtrl) {
    key = wgetch(hWndCtrl->hWnd);
    gInterface->onKeyPressed(key, hWndCtrl);
}

/* Draws text in window. */

void PseudoGUIManager::drawText(ExtWindowCtrl *pExtWnd, char* text, int x, int y) {
    if(y > pExtWnd->hHeight - 20)
        return;

    move(y, 0);
    wclrtoeol(pExtWnd->hWnd);
    mvwprintw(pExtWnd->hWnd, y, x, "%s", text);

    box(pExtWnd->hWnd, 0, 0);
    mvwprintw(
        pExtWnd->hWnd,
        0, (pExtWnd->hWidth - strlen(pExtWnd->hTitle) - 4) / 2,
        "\u2524 %s \u251c", pExtWnd->hTitle
    );

    wrefresh(pExtWnd->hWnd);
}

/* Create window area with titlebar. */

ExtWindowCtrl* PseudoGUIManager::createWindow(char* id, char* title, int width, int height, bool alignCenter) {
    ExtWindowCtrl *pExtWnd = new ExtWindowCtrl(id);

    /*                       int     int   int int
     * WINDOW* wnd = newwin(height, width,  y , x ) <-- creates new window inside console screen
     */

    int realWidth = 12;
    int realHeight = 12;

    getmaxyx(stdscr, gActiveHeight, gActiveWidth);

    if(alignCenter) {
        if(width > gActiveWidth) {
            realWidth = gActiveWidth - 6;
        } else if(width <= 0) {
            realWidth = gActiveWidth + width;
        } else {
            realWidth = width;
        }

        if(height > gActiveHeight) {
            realHeight = gActiveHeight - 6;
        } else if (height <= 0){
            realHeight = gActiveHeight + height - 1;
        } else {
            realHeight = height;
        }

        pExtWnd->hWnd = newwin(realHeight, realWidth,
                               ((gActiveHeight - realHeight) / 2) + 1,
                               (gActiveWidth - realWidth) / 2);
    } else {
        realWidth = width;
        realHeight = height;
        pExtWnd->hWnd = newwin(realHeight, realWidth, 1, 0);
    }

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
    return pExtWnd;
}

void PseudoGUIManager::clearWindow(ExtWindowCtrl* pExtWnd) {
    wclear(pExtWnd->hWnd);

    box(pExtWnd->hWnd, 0, 0);                   // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        pExtWnd->hWnd,
        0, (pExtWnd->hWidth - strlen(pExtWnd->hTitle) - 4) / 2,
        "\u2524 %s \u251c", pExtWnd->hTitle
    );
    wrefresh(pExtWnd->hWnd);
}

/* Freeing ncurses:
 * 'delete [object];' calling '[ClassName]::~[ClassName]()' function.
 */

PseudoGUIManager::~PseudoGUIManager() {
    endwin();
    delscreen((SCREEN*)stdscr);
}
