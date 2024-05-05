#include "pguiman.h"
#include <locale.h>

char key;

/* Initializes ncurses screen. */

PseudoGUIManager::PseudoGUIManager(IPseudoGUIManager *interface) {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    if (has_colors()) {
        use_default_colors();
        start_color();
        init_pair(1, COLOR_RED, -1);
    }
    noecho();
    curs_set(0);
    gInterface = interface;
}

/* Shows version info and copyright in console top area. */

void PseudoGUIManager::showTopVersionInfo() {
    getmaxyx(stdscr, gActiveHeight, gActiveWidth);
    char verInfoStr[] = "Tinelix OpenDSS v. 0.0.1. Copyright (C) 2024 Dmitry Tretyakov\n";
    move(0, (gActiveWidth - strlen(verInfoStr)) / 2);
    printw("%s", verInfoStr);
    refresh();
}

/* Listens pressed keys on keyboard. */

void PseudoGUIManager::listenKeyboard() {
    key = getch();
    gInterface->onKeyPressed(key);
}

/* Draws text in window. */

void PseudoGUIManager::drawText(ExtWindow *wnd, char* text, int x, int y) {
    move(y, 0);
    wclrtoeol((WINDOW*)wnd);
    mvwprintw((WINDOW*)wnd, y, x, "%s", text);

    box((WINDOW*)wnd, 0, 0);
    mvwprintw((WINDOW*)wnd, 0, (wnd->width - strlen(wnd->title)) / 2, "┤ %s ├", wnd->title);
    wrefresh((WINDOW*)wnd);
}

/* Create window area with titlebar. */

ExtWindow* PseudoGUIManager::createWindow(char* title, int width, int height, bool alignCenter) {
    ExtWindow *wnd;

    // newwin(height, width, y, x)

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

        wnd = (ExtWindow*)newwin(height, width, ((gActiveHeight - height) / 2 + 1), (gActiveWidth - width) / 2);
    } else {
        wnd = (ExtWindow*)newwin(height, width, 1, 0);
    }

    sprintf(wnd->title, "%s", title);
    wnd->width = width;
    wnd->height = height;
    box((WINDOW*)wnd, 0, 0);
    mvwprintw((WINDOW*)wnd, 0, (width - strlen(wnd->title)) / 2, "┤ %s ├", wnd->title);
    wrefresh((WINDOW*)wnd);
    return wnd;
}

/* Freeing ncurses */

PseudoGUIManager::~PseudoGUIManager() {
    endwin();
}
