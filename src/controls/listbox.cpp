#include "listbox.h"
#include "extwnd.h"

ListBoxCtrl::ListBoxCtrl(ExtWindowCtrl* pParent, int pItemCount) {
    gParent = pParent;
    gItemCount = pItemCount;
    gSelectionIndex = 0;
    hType = 1;
}

int ListBoxCtrl::getSelectionIndex() {
    return gSelectionIndex;
}

void ListBoxCtrl::setSelectionIndex(int index) {
    gSelectionIndex = index;
}

int ListBoxCtrl::getItemCount() {
    return gItemCount;
}

void ListBoxCtrl::onKeyPressed(char k) {
    if((int)k == 2 || (int)k == 3) {
        int index = getSelectionIndex();

        if((int)k == 3 && index > 0) { // top arrow key
            index--;
        } else if((int)k == 2 && index < getItemCount()) { // bottom arrow key
            index++;
        }

        int list_index = getSelectionIndex() + 2;
        if(index >= 0 && index < getItemCount()) {
            drawListPointer(
                2,
                list_index,
                false
            );
        }

        if(index <= getItemCount() - 1) {
            setSelectionIndex(index);
            list_index = getSelectionIndex() + 2;
        }

        if(index < getItemCount()) {
            drawListPointer(
                        2,
                        list_index,
                        true
            );
        }
    }
}

void ListBoxCtrl::drawListPointer(int x, int y, bool isVisible) {
    if(isVisible) {
        mvwaddch(gParent->hWnd, y, x, '*');  // Adds a star pointer to the specified position
        mvwchgat(gParent->hWnd, y, x, gParent->hWidth - 4, A_BOLD, 1, NULL);
    } else {
        mvwaddch(gParent->hWnd, y, x, ' ');  // Removes a star pointer to the specified position
        mvwchgat(gParent->hWnd, y, x, gParent->hWidth - 4, A_NORMAL, 2, NULL);
    }

    wrefresh(gParent->hWnd);
}
