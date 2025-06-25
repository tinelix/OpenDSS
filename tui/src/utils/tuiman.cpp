#include <utils/tuiman.h>
#include <opendss.h>

bool hKbdListening;

TUIManager::TUIManager() {

    screen = initscr();

    keypad(screen, true);

    noecho();

    curs_set(0);

    if(has_colors()) {
        start_color();

        #ifndef _PDCURSES_256
            init_pair(1,  COLOR_LIGHT_CYAN, COLOR_BLACK     );
            init_pair(3,  COLOR_WHITE,      COLOR_DARK_GRAY );
            init_pair(4,  COLOR_WHITE,      COLOR_RED       );
            init_pair(5,  COLOR_WHITE,      COLOR_GREEN     );
            init_pair(6,  COLOR_GREEN,      COLOR_WHITE     );
            init_pair(7,  COLOR_RED,        COLOR_BLACK     );
            init_pair(8,  COLOR_RED,        COLOR_BLACK     );
            init_pair(9,  COLOR_BLACK,      COLOR_WHITE     );
            init_pair(10, COLOR_BLACK,      COLOR_BLACK     );

            bkgd(COLOR_PAIR(3));
        #else

        #endif
    }

    getmaxyx(screen, gActiveHeight, gActiveWidth);
}

TUIManager::~TUIManager() {

}

void TUIManager::showTopBanner() {
    char* verStr        = OpenDSS::getVersion();
    char* yearsStr      = OpenDSS::getReleaseYears();
    char* fmtVerInfoStr = (char*)malloc(92 * sizeof(char));
    const char* verInfoStr =
        "Tinelix OpenDSS v. %s | Copyright (C) %s Dmitry Tretyakov";
    int x;

    sprintf(fmtVerInfoStr, verInfoStr, verStr, yearsStr);

    move(0, 0);

    attron(A_BOLD | COLOR_PAIR(1));
    bkgdset(COLOR_PAIR(1));

    for(x = 0; x < gActiveWidth; x++) {
        mvaddch(0, x, ' ');
    }

    move(0, (gActiveWidth - strlen(fmtVerInfoStr)) / 2);

    printw(fmtVerInfoStr);

    move(0, gActiveWidth);

    attroff(A_BOLD | COLOR_PAIR(1));
    bkgdset(COLOR_PAIR(3));

    refresh();
}

void TUIManager::listenKbd() {
    hKbdListening = true;
    while(hKbdListening) {
        char key = getch();
        onKeyPressed(key);
    }
}

void TUIManager::onKeyPressed(char key) {
    if(key == 'q')
        hKbdListening = false;
}

WINDOW* TUIManager::getScreen() {
    return screen;
}
