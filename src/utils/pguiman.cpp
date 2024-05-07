#include "pguiman.h"
#include <curses.h>
#include <locale.h>

char key;

/* Initializes ncurses screen. */

PseudoGUIManager::PseudoGUIManager(IPseudoGUIManager *interface) {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);                       // <-- enables arrow pressed keys handling

    noecho();                                   /* <-- disables escaping of characters typed on the
                                                       keyboard                                     */
    curs_set(0);                                // <-- makes the terminal cursor invisible

    // Creates a color that will be used later if its output is supported by the terminal.

    if(has_colors()) {
        start_color();

        init_color(COLOR_BLUE, 184, 142, 12);  //  <-- create RGB value for COLOR_BLUE variable
        init_pair(1, COLOR_WHITE, COLOR_BLUE);

        init_color(COLOR_GRAY, 150, 150, 150);  //  <-- create RGB value for COLOR_GRAY variable
        init_pair(2, COLOR_WHITE, COLOR_GRAY);

        init_color(COLOR_DEEP_BLACK, 0, 0, 0);  //  <-- create RGB value for COLOR_DEEP_BLACK variable
        init_pair(3, COLOR_WHITE, COLOR_DEEP_BLACK);
    }

    bkgd(COLOR_PAIR(3));

    gInterface = interface;
}

/* Shows version info and copyright in console top area. */

void PseudoGUIManager::showTopVersionInfo() {
    getmaxyx(stdscr, gActiveHeight, gActiveWidth);

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

void PseudoGUIManager::listenKeyboard(ExtWindowCtrl *pExtWnd) {
    key = wgetch(pExtWnd->hWnd);
    gInterface->onKeyPressed(key, pExtWnd);
}

/* Draws text in window. */

void PseudoGUIManager::drawText(ExtWindowCtrl *pExtWnd, char* text, int x, int y) {
    move(y, 0);
    wclrtoeol(pExtWnd->hWnd);                   // <-- clearing line (including window vetical borders)
    mvwprintw(pExtWnd->hWnd, y, x, "%s", text); // <-- overwrite line

    box(pExtWnd->hWnd, 0, 0);                   // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        pExtWnd->hWnd,
        0, (pExtWnd->hWidth - strlen(pExtWnd->hTitle)) / 2,
        "\u2524 %s \u251c", pExtWnd->hTitle
    );

    /*           WINDOW*
     * wrefresh( window ) <-- updates the contents of the window for display
     */

    wrefresh(pExtWnd->hWnd);
}

/* Create window area with titlebar. */

ExtWindowCtrl* PseudoGUIManager::createWindow(char* title, int width, int height, bool alignCenter) {
    ExtWindowCtrl *pExtWnd = new ExtWindowCtrl();

    /*                       int     int   int int
     * WINDOW* wnd = newwin(height, width,  y , x ) <-- creates new window inside console screen
     */

    if(alignCenter) {
        if(width < gActiveWidth) {
            width = gActiveWidth - 6;
        } else if(width == 0) {
            width = gActiveWidth;
        }

        if(height < gActiveHeight) {
            height = gActiveHeight - 6;
        } else {
            height = gActiveHeight - 1;
        }

        pExtWnd->hWnd = newwin(height, width, ((gActiveHeight - height) / 2 + 1), (gActiveWidth - width) / 2);
    } else {
        pExtWnd->hWnd = newwin(height, width, 1, 0);
    }

    sprintf(pExtWnd->hTitle, "%s", title);      // <-- store window text in ExtWindow object

    pExtWnd->hWidth = width;
    pExtWnd->hHeight = height;

    box(pExtWnd->hWnd, 0, 0);                   // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        pExtWnd->hWnd,
        0, (width - strlen(pExtWnd->hTitle)) / 2,
        "\u2524 %s \u251c", pExtWnd->hTitle
    );

    wbkgd(pExtWnd->hWnd, COLOR_PAIR(2));

    keypad(pExtWnd->hWnd, true);

    wrefresh(pExtWnd->hWnd);
    return pExtWnd;
}

/* Freeing ncurses:
 * 'delete [object];' calling '[ClassName]::~[ClassName]()' function.
 */

PseudoGUIManager::~PseudoGUIManager() {
    endwin();
}
