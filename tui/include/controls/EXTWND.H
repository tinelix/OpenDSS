#ifndef _OPENDSS_CONTROLS_EXTWND_H
#define _OPENDSS_CONTROLS_EXTWND_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <opendss.h>

class ExtWindowCtrl {
    public:
        ExtWindowCtrl();
        ExtWindowCtrl(const char* pid);
        ExtWindowCtrl(const char* pId, ExtWindowCtrl* pParent);

        char id[60];
        char* hTitle;
        unsigned int gWidth;
        unsigned int gHeight;

        ExtWindowCtrl* gParent;

        void createWnd(int x, int y);
    protected:
        WINDOW* hWnd;
        WINDOW* hScreen;
        ExtWindowCtrl** hChildWnds;
};

#endif