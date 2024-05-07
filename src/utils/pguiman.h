#ifndef OPENDSS_UTILS_PGUIMAN_H
#define OPENDSS_UTILS_PGUIMAN_H

#include <ncurses.h>                // Linking ncurses library for creating pseudo-GUIs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <interfaces/pguiman.h>

#define COLOR_GRAY 242
#define COLOR_DEEP_BLACK 16

class ExtWindow {
public:
    char hTitle[255];
    int hWidth, hHeight;
    WINDOW* hWnd;
};

class PseudoGUIManager {
    public:
        int gActiveWidth, gActiveHeight;
        PseudoGUIManager(IPseudoGUIManager *interface);
        ~PseudoGUIManager();
        void showTopVersionInfo();
        void listenKeyboard();
        ExtWindow* createWindow(char* title, int width, int height, bool alignCenter);
        void drawText(ExtWindow *wnd, char* text, int x, int y);
        void drawListPointer(ExtWindow *pExtWnd, int x, int y, bool isVisible);
    private:
        IPseudoGUIManager *gInterface;
        ExtWindow    *gWnd;
        char         gWndTitle[255];
};

#endif // OPENDSS_UTILS_PGUIMAN_H
