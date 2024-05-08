#include "listbox.h"
#include <cstring>
#include <curses.h>

ListBoxCtrl::ListBoxCtrl(ExtWindowCtrl* pParent, int pItemCount) {
    gParent = pParent;
    gListItems = (ListItem**) malloc(pItemCount * sizeof(ListItem));
    gSelectionIndex = 0;
    hType = 1;
    gPageNumber = 0;
    gItemCount = pItemCount;
}

ListBoxCtrl::~ListBoxCtrl() {
    free(gListItems);
}

void ListBoxCtrl::recreate(int pItemCount) {
    for(int y = 0; y <= hHeight; y++) {
        wmove(gParent->hWnd, hY+y, 0);
        wclrtoeol(gParent->hWnd);
    }
    free(gListItems);
    gListItems = (ListItem**) malloc(pItemCount * sizeof(ListItem));
    gPageNumber = 0;
    gSelectionIndex = 0;
    gItemCount = pItemCount;
    box(gParent->hWnd, 0, 0);                   // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        gParent->hWnd,
        0, (gParent->hWidth - strlen(gParent->hTitle)) / 2,
        "\u2524 %s \u251c", gParent->hTitle
    );
}

void ListBoxCtrl::addListItem(int index, ListItem* item) {
    if(item == NULL) return;
    gListItems[index] = item;
    if(index <= hHeight)  {
        mvwprintw(gParent->hWnd, (index % (hHeight + 1)) + hY, 4, "%s", item->title);
    }
}

int ListBoxCtrl::getSelectionIndex() {
    if(gSelectionIndex > 0)
        return (gPageNumber * hHeight) + gSelectionIndex;
    else
        return 0;
}

void ListBoxCtrl::setSelectionIndex(int index) {
    gSelectionIndex = index % hHeight;
}

int ListBoxCtrl::getVirtualSelectionIndex() {
    char selection_label[80];
    sprintf(
        selection_label, "%d / %d",
        (gPageNumber * hHeight) + gSelectionIndex + 1, gItemCount
    );
    mvwprintw(
        gParent->hWnd, 2, gParent->hWidth - strlen(selection_label) - 5,
        "   %s", selection_label
    );
    if(gSelectionIndex > 0)
        return gSelectionIndex;
    else
        return 0;
}

int ListBoxCtrl::getItemCount() {
    return gItemCount;
}

void ListBoxCtrl::goToPage(int pPageNumb) {
    if(pPageNumb < 0)
        gPageNumber = 0;
    if(gItemCount < (pPageNumb) * hHeight) return;
    gPageNumber = pPageNumb;
    for(int y = 0; y <= hHeight; y++) {
        wmove(gParent->hWnd, hY+y, 0);
        wclrtoeol(gParent->hWnd);
        ListItem* item = gListItems[(pPageNumb * hHeight) + y];
        if(item != NULL)  {
            mvwprintw(gParent->hWnd, y + hY, 4, "%s", item->title);
        }
    }

    box(gParent->hWnd, 0, 0);                   // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        gParent->hWnd,
        0, (gParent->hWidth - strlen(gParent->hTitle)) / 2,
        "\u2524 %s \u251c", gParent->hTitle
    );
}

void ListBoxCtrl::onKeyPressed(char k) {
    if((int)k == 2 || (int)k == 3) {
        int index = getVirtualSelectionIndex();

        if((int)k == 3) { // top arrow key
            if(index > 0) {
                index--;
            }
            if(index <= 0 && gPageNumber > 0) {
                goToPage(gPageNumber - 1);
                index = hHeight;
            }
        } else if((int)k == 2) { // bottom arrow key
            if(gItemCount - 1 > getSelectionIndex()) {
                index++;
            }
            if(index >= hHeight) {
                index = 0;
                if(index <= 0) {
                    goToPage(gPageNumber + 1);
                }
            }
        }

        int list_index = getVirtualSelectionIndex() + hY;
        if(index >= 0 && index < getItemCount()) {
            drawListPointer(
                hX,
                list_index,
                false
            );
        }

        if(index <= getItemCount() - 1) {
            gSelectionIndex = index;
            list_index = getVirtualSelectionIndex() + hY;
        }

        if(index < getItemCount()) {
            drawListPointer(
                        hX,
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
