#include "pguiman.h"
#include <locale.h>

char key;

PseudoGUIManager::PseudoGUIManager(IPseudoGUIManager *interface) {
    setlocale(LC_ALL, "");
    initscr();
    gInterface = interface;
}

void PseudoGUIManager::showTopVersionInfo() {
    getmaxyx(stdscr, gActiveHeight, gActiveWidth);
    noecho();
    curs_set(0);
    char verInfoStr[] = "Tinelix OpenDSS v. 0.0.1. Copyright (C) 2024 Dmitry Tretyakov\n";
    move(0, (gActiveWidth - strlen(verInfoStr)) / 2);
    printw("%s", verInfoStr);
    refresh();
}

void PseudoGUIManager::listenKeyboard() {
    key = getch();
    gInterface->onKeyPressed(key);
}

WINDOW* PseudoGUIManager::createWindow(char* title, int width, int height, bool alignCenter) {
    WINDOW *wnd;

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
            width = gActiveHeight - 1;
        }

        wnd = newwin(height, width, ((gActiveHeight - height) / 2 + 1), (gActiveWidth - width) / 2);
    } else {
        wnd = newwin(height, width, 1, 0);
    }
    box(wnd, 0, 0);
    mvwprintw(wnd, 0, (width - strlen(title)) / 2, "┤ %s ├", title);
    wrefresh(wnd);
    return wnd;
}

PseudoGUIManager::~PseudoGUIManager() {
    endwin();
}
