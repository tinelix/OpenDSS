#ifndef OPENDSS_CONTROLS_LISTBOX_H
#define OPENDSS_CONTROLS_LISTBOX_H

#include <controls/extwnd.h>
#include <utils/uictrl.h>
#include <ncurses.h>

class ListBoxCtrl : UIControl {
    public:
        ListBoxCtrl(ExtWindowCtrl* pParent, int pItemCount);
        int getSelectionIndex();
        void setSelectionIndex(int index);
        void onKeyPressed(char k);
        void drawListPointer(int x, int y, bool isVisible);
        int getItemCount();
    private:
        int gSelectionIndex, gItemCount;
        ExtWindowCtrl* gParent;
};

#endif
