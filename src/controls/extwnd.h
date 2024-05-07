#ifndef OPENDSS_CONTROLS_EXTWND_H
#define OPENDSS_CONTROLS_EXTWND_H

#include <ncurses.h>
#include <utils/uictrl.h>
#include <stdio.h>
#include <stdlib.h>

class ExtWindowCtrl {
public:
    ExtWindowCtrl();
    ~ExtWindowCtrl();
    char hTitle[255], id[60];
    int hWidth, hHeight;
    WINDOW* hWnd;
    UIControl** hCtrls;
    void addControl(UIControl* pCtrl);
    int getControlsSize();
};

#endif
