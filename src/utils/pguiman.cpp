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

#include "pguiman.h"

char key;

#ifdef _WIN32
int* winver;
#endif

/* Initializes ncurses screen. */

PseudoGUIManager::PseudoGUIManager(IPseudoGUIManager* pInterface) {
    setlocale(LC_ALL, "");                      /* <-- set locale for correct non-ASCII characters
                                                       displaying */
    screen = initscr();                         /* <-- temporally clearing command prompt and initializes
                                                       empty screen of ncurses */
    keypad(screen, true);                       // <-- enables arrow pressed keys handling

    noecho();                                   /* <-- disables escaping of characters typed on the
                                                       keyboard                                     */
    curs_set(0);                                // <-- makes the terminal cursor invisible

    // Creates a color that will be used later if its output is supported by the terminal.

    if (has_colors()) {
        start_color();
        #ifdef _WIN32
            winver = OpenDSSVersion::getWindowsVersion();

            if (winver[0] <= 10 && winver[2] <= 19041) { // if Windows lower than 10 2004
                init_pair(1, COLOR_CYAN, COLOR_BLACK);
                init_color(COLOR_CYAN, 768, 768, 768);
                init_pair(2, COLOR_WHITE, COLOR_BLACK);
                init_color(COLOR_WHITE, 768, 768, 768);
                init_pair(3, COLOR_WHITE, COLOR_BLACK);
                init_pair(4, COLOR_WHITE, COLOR_RED);
                init_pair(5, COLOR_WHITE, COLOR_GREEN);
                init_color(COLOR_GREEN, 768, 768, 768);
                init_pair(6, COLOR_GREEN, COLOR_BLACK);
                init_pair(7, COLOR_GREEN, COLOR_BLACK);
                init_color(COLOR_RED, 768, 768, 768);
                init_pair(8, COLOR_RED, COLOR_BLACK);
            } else {
        #endif
                init_color(COLOR_LIGHT_WHITE, 768, 768, 768);
                init_color(COLOR_BLUE_SKY, 0, 142, 768);  //  <-- create RGB value for COLOR_BLUE_SKY variable
                init_pair(1, COLOR_LIGHT_WHITE, COLOR_BLUE_SKY);
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
        #ifdef _WIN32
            }
        #endif
    }

    bkgd(COLOR_PAIR(3));

    gInterface = pInterface;

    getmaxyx(screen, gActiveHeight, gActiveWidth);
}

/* Shows version info and copyright in console top area. */

void PseudoGUIManager::showTopVersionInfo() {

    char* verStr = OpenDSSVersion::getVersion();
    const char* verInfoStr = "Tinelix OpenDSS v. %s | Copyright (C) 2024 Dmitry Tretyakov";

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

    move(0, (gActiveWidth - strlen(verInfoStr) - strlen(verStr) + 2) / 2);
    printw(verInfoStr, verStr);

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

WINDOW* PseudoGUIManager::getScreen() {
    return screen;
}

/* Freeing ncurses:
 * 'delete [object];' calling '[ClassName]::~[ClassName]()' function.
 */

PseudoGUIManager::~PseudoGUIManager() {
    endwin();
    delscreen((SCREEN*)screen);
}