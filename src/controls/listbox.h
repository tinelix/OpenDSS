#ifndef OPENDSS_CONTROLS_LISTBOX_H
#define OPENDSS_CONTROLS_LISTBOX_H

#include "extwnd.h"
#include "../utils/uictrl.h"
#include "../utils/extstr.h"

#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#else
    #include <ncurses.h>
#endif

struct ListItem {
    char title[128];
    void* ptr;
};

class ListBoxCtrl : UIControl {
    public:
        ListBoxCtrl(ExtWindowCtrl* pParent, int pItemCount);
        ~ListBoxCtrl();
        void addListItem(int index, ListItem* item);
        int getSelectionIndex();
        void setSelectionIndex(int index);
        void onKeyPressed(char k);
        void drawListPointer(int x, int y, bool isVisible);
        void goToPage(int pPageNumber);
        void recreate(int pItemCount);
        int getItemCount();
        int hX, hY, hWidth, hHeight;
        struct ListItem**   gListItems;
    protected:
        int getVirtualSelectionIndex();
        ExtWindowCtrl* gParent;
        int gSelectionIndex, gPageNumber, gItemCount;

};

#endif
