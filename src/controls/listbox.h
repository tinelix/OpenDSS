#ifndef OPENDSS_CONTROLS_LISTBOX_H
#define OPENDSS_CONTROLS_LISTBOX_H

#include "extwnd.h"
#include "../utils/uictrl.h"
#include <ncurses.h>

class ListBoxCtrl : UIControl {
    public:
        ListBoxCtrl(ExtWindowCtrl* pParent, int pItemCount);
        int getSelectionIndex();
        void setSelectionIndex(int index);
        void onKeyPressed(char k);
        void drawListPointer(int x, int y, bool isVisible);
        int getItemCount();
        void setItemCount(int count);
        int hX, hY, hWidth, hHeight;
    protected:
        ExtWindowCtrl* gParent;
        int gSelectionIndex, gItemCount;

};

#endif
