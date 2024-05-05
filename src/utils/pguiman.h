#ifndef OPENDSS_UTILS_PGUIMAN_H
#define OPENDSS_UTILS_PGUIMAN_H

#include <ncurses.h>                // Linking ncurses library for creating pseudo-GUIs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <interfaces/pguiman.h>

class PseudoGUIManager {
    public:
        int gActiveWidth, gActiveHeight;
        PseudoGUIManager(IPseudoGUIManager *interface);
        ~PseudoGUIManager();
        void showTopVersionInfo();
        void listenKeyboard();
        WINDOW* createWindow(char* title, int width, int height, bool alignCenter);
    private:
        IPseudoGUIManager *gInterface;
};

#endif // OPENDSS_UTILS_PGUIMAN_H
