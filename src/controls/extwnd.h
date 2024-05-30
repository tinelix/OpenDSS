#ifndef OPENDSS_CONTROLS_EXTWND_H
#define OPENDSS_CONTROLS_EXTWND_H

#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#else
    #include <ncurses.h>
#endif

#include "uictrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ExtWindowCtrl {
    public:
        ExtWindowCtrl();
        ExtWindowCtrl(char* pId);
        ~ExtWindowCtrl();
        char hTitle[255], id[60];
        int hWidth, hHeight;
        WINDOW* hWnd;
        UIControl** hCtrls;
        ExtWindowCtrl** hChildWnds;
        void addControl(UIControl* pCtrl);
        void addChildWindow(char* id, char* title, int width, int height, int x, int y);
        int getControlsSize();
        int getChildWindowsSize();
        void redraw();
        void clear();
        void freeWnd();
    private:
        int gCtrlSize;
        int gChildWndsSize;
};

#endif
