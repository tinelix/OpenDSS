#ifndef OPENDSS_UTILS_PGUIMAN_H
#define OPENDSS_UTILS_PGUIMAN_H

#include <ncurses.h>                // Linking ncurses library for creating pseudo-GUIs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../interfaces/pguiman.h"
#include "../controls/extwnd.h"

#define COLOR_GRAY 242
#define COLOR_DEEP_BLACK 16

class PseudoGUIManager {
    public:
        int gActiveWidth, gActiveHeight;
        PseudoGUIManager(IPseudoGUIManager *interface);
        ~PseudoGUIManager();
        void showTopVersionInfo();
        void listenKeyboard();
        void listenKeyboard(ExtWindowCtrl *pExtWnd);
        ExtWindowCtrl* createWindow(char* title, int width, int height, bool alignCenter);
        void clearWindow(ExtWindowCtrl* pExtWnd);
        void drawText(ExtWindowCtrl *wnd, char* text, int x, int y);
    private:
        IPseudoGUIManager   *gInterface;
        ExtWindowCtrl       *gWnd;
        char                gWndTitle[255];
};

#endif // OPENDSS_UTILS_PGUIMAN_H
