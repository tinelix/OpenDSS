#include <controls/extwnd.h>

ExtWindowCtrl::ExtWindowCtrl() {
    gWidth = 8;
    gHeight = 8;
    hChildWnds =
        (ExtWindowCtrl**)malloc(sizeof(ExtWindowCtrl) * 16);
}

ExtWindowCtrl::ExtWindowCtrl(const char* pId) {
    strcpy(id, pId);

    gWidth = 8;
    gHeight = 8;
    hChildWnds =
        (ExtWindowCtrl**)malloc(sizeof(ExtWindowCtrl) * 16);
}

ExtWindowCtrl::ExtWindowCtrl(const char* pId, ExtWindowCtrl* pParent) {
    strcpy(id, pId);

    gWidth = 8;
    gHeight = 8;
    hChildWnds =
        (ExtWindowCtrl**)malloc(sizeof(ExtWindowCtrl) * 16);
    gParent = pParent;
}

void ExtWindowCtrl::createWnd(int x, int y) {
    hScreen = stdscr;

    hWnd = newwin(gWidth, gHeight, y, x);
    box(hWnd, 0, 0);
    wbkgd(hWnd, COLOR_PAIR(2));

    keypad(hWnd, true);
    wrefresh(hWnd);
}
