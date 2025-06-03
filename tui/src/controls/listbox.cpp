/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -----------------------------------------------------------------------------
 *  Copyright © 2024, 2025 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License 3 (or any later version)
 *  and/or Apache License 2.
 *
 *  See the following files in repository directory for the precise terms and
 *  conditions of either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing
 *  information (in the opening comment of each file).
 */

#include "listbox.h"

ListBoxCtrl::ListBoxCtrl(ExtWindowCtrl* pParent, int pItemCount, bool pTrackPos) {
    gParent = pParent;
    gListItems = (ListItem**)malloc(pItemCount * sizeof(ListItem));
    gSelectionIndex = 0;
    hType = 1;
    gPageNumber = 0;
    hExpandedItemIndex = 0;
    hSubItemCount = 0;
    gItemCount = pItemCount;
    gTrackPos = pTrackPos;
}

ListBoxCtrl::~ListBoxCtrl() {
    for (int i = 0; i < gItemCount; i++) {
        free(gListItems[i]->subItems);
    }
    free(gListItems);
}

void ListBoxCtrl::recreate(int pItemCount) {
    for (int y = hY; y <= (hHeight + hY); y++) {
        for (int x = hX; x <= (hWidth + hX); x++) {
            mvwaddch(gParent->hWnd, y, x, ' ');
        }
    }
    wrefresh(gParent->hWnd);
    for (int i = 0; i < gItemCount; i++) {
        free(gListItems[i]->subItems);
    }
    free(gListItems);
    gListItems = (ListItem**)malloc(pItemCount * sizeof(ListItem));
    gPageNumber = 0;
    gSelectionIndex = 0;
    gItemCount = pItemCount;
}

void ListBoxCtrl::addListItem(int index, ListItem* item) {
    if (item == NULL) return;

    gListItems[index] = item;

    char shortestTitle[128];
    strcpy(shortestTitle, item->title);
    if ((int)strlen(item->title) > hWidth - 2) {
        ExtString::strcut(shortestTitle, hWidth + 16, -1);
    }
    if (index <= hHeight) {
        mvwprintw(
            gParent->hWnd, 
            (index % (hHeight + 1)) + hY, 
            hX + 2, 
            "%s", 
            item->title
        );
    }

    if (index == 0) {
        drawListPointer(
            hX,
            hY,
            true
        );
    }
}

int ListBoxCtrl::getSelectionIndex() {
    if (gSelectionIndex > 0)
        return (gPageNumber * hHeight) + gSelectionIndex;
    else
        return 0;
}

void ListBoxCtrl::setSelectionIndex(int index) {
    if (hHeight > 0)
        gSelectionIndex = index % hHeight;
}

int ListBoxCtrl::getVirtualSelectionIndex() {
    if (gTrackPos) {
        char selection_label[80];
        #ifdef _MSVC2005G
            sprintf_s(
                selection_label, "%d / %d",
                80,
                (gPageNumber * hHeight) + gSelectionIndex + 1, gItemCount
            );
        #else
            sprintf(
                selection_label, "%d / %d",
                (gPageNumber * hHeight) + gSelectionIndex + 1, gItemCount
            );
        #endif
        mvwprintw(
            gParent->hWnd, 2, gParent->hWidth - strlen(selection_label) - 5,
            "   %s", selection_label
        );
    }
    if (gSelectionIndex > 0)
        return gSelectionIndex;
    else
        return 0;
}

struct ListItem** ListBoxCtrl::getItems() {
    return gListItems;
}

int ListBoxCtrl::getItemsCount() {
    return gItemCount;
}

int ListBoxCtrl::getSubItemsCount(int pIndex) {
    ListItem* item = gListItems[pIndex];
    return item->subItemsCount;
}

int ListBoxCtrl::getPageNumber() {
    return gPageNumber;
}

void ListBoxCtrl::goToPage(int pPageNumb) {
    if (pPageNumb < 0)
        gPageNumber = 0;

    if (gItemCount < (pPageNumb)*hHeight) return;
    gPageNumber = pPageNumb;

    for (int y = hY; y <= (hHeight + hY); y++) {
        for (int x = hX; x <= (hWidth + hX); x++) {
            mvwaddch(gParent->hWnd, y, x, ' ');
        }
    }

    for (int y = 0; y <= hHeight; y++) {
        ListItem *item = gListItems[(pPageNumb * hHeight) + y];
        if (item != NULL) {
            mvwprintw(gParent->hWnd, y + hY, 4, "%s", item->title);
        }
    }
}

void ListBoxCtrl::onKeyPressed(char k) {
    int virtIndex = getVirtualSelectionIndex();
    if ((int)k == 2 || (int)k == 3) {

        if ((int)k == 3) { // top arrow key
            if (virtIndex > 0) {
                virtIndex--;
            }
            if (virtIndex <= 0 && gPageNumber > 0) {
                goToPage(gPageNumber - 1);
                virtIndex = hHeight;
            }
        }
        else if ((int)k == 2) { // bottom arrow key
            if (gItemCount - 1 > getSelectionIndex()) {
                virtIndex++;
            }
            if (virtIndex >= hHeight) {
                virtIndex = 0;
                if (virtIndex <= 0) {
                    goToPage(gPageNumber + 1);
                }
            }
        }

        int list_index = getVirtualSelectionIndex() + hY;
        if (virtIndex >= 0 && virtIndex < getItemsCount()) {
            drawListPointer(
                hX, list_index, false
            );
        }

        if (virtIndex <= getItemsCount() - 1) {
            gSelectionIndex = virtIndex;
            list_index = getVirtualSelectionIndex() + hY;
        }

        if (virtIndex < getItemsCount()) {
            drawListPointer(
                hX, list_index, true
            );
        }
    }
    else if (k == (int)4 || k == (int)5) {
        int virtIndex = getVirtualSelectionIndex();
        int index = getSelectionIndex();

        drawListPointer(
            hX, hY + virtIndex, '^', k == (int)5
        );

        expand(index, k == (int)5);
    }
    else if (k == (int)82) {
        if (gPageNumber * hHeight < getItemsCount()) {
            goToPage(gPageNumber + 1);
            drawListPointer(
                hX, hY + virtIndex, '^', k == (int)5
            );
        }
    }
    else if (k == (int)83) {
        if (gPageNumber > 0) {
            goToPage(gPageNumber - 1);
            drawListPointer(
                hX, hY + virtIndex, '^', k == (int)5
            );
        }

    }
}

void ListBoxCtrl::expand(int pIndex, bool status) {

    int pIndex2 = pIndex;

    if (pIndex > hExpandedItemIndex &&
        pIndex <= hExpandedItemIndex + hSubItemCount) {
        return;
    }
    else if (pIndex > hExpandedItemIndex + hSubItemCount) {
        pIndex2 -= hSubItemCount;
    }

    ListItem* parentItem = gListItems[pIndex2];

    int subItemsCount = gListItems[pIndex2]->subItemsCount;

    if (subItemsCount == 0) return;

    if ((status && !parentItem->expanded)
        || (!status && parentItem->expanded)) {

        for (int y = hY; y <= (hHeight + hY); y++) {
            for (int x = hX; x <= (hWidth + hX); x++) {
                mvwaddch(gParent->hWnd, y, x, ' ');
            }
        }
    }

    wrefresh(gParent->hWnd);

    char shortestTitle[384];

    if (status && !parentItem->expanded) {
        if (expanded && pIndex2 != hExpandedItemIndex) {
            expand(hExpandedItemIndex, false);   // <-- recursive call if there is an expanded item
        }

        hExpandedItemIndex = pIndex2;
        hSubItemCount = subItemsCount;

        // Before child items

        for (int y = 0; y <= (pIndex2 % hHeight); y++) {
            ListItem* item = gListItems[(gPageNumber * hHeight) + y];
            if (item != NULL) {
                #ifdef _MSVC2005G
                    strcpy_s(shortestTitle, 288, item->title);
                #else
                    strcpy(shortestTitle, item->w_title);
                #endif
                if ((int)strlen(parentItem->title) > hWidth - 2) {
                    ExtString::strcut(shortestTitle, hWidth + 16, -1);
                }

                mvwprintw(gParent->hWnd, y + hY, 4, "%s", shortestTitle);
            }
        }

        int firstSubItemY = (pIndex2 % hHeight) + 1;
        int endOfSubItems = firstSubItemY + subItemsCount;

        // Child items

        for (int y = firstSubItemY; y < endOfSubItems; y++) {
            ListItem* item = gListItems[pIndex2]->subItems[y - firstSubItemY];
            if (item != NULL) {
                for (int x = hX; x <= hWidth; x++) {
                    mvwaddch(gParent->hWnd, y + hY, x, ' ');
                }
                #ifdef _MSVC2005G
                    strcpy_s(shortestTitle, 288, item->title);
                #else
                    strcpy(shortestTitle, item->w_title);
                #endif
                if ((int)strlen(parentItem->title) > hWidth - 6) {
                    ExtString::strcut(shortestTitle, hWidth + 12, -1);
                }

                if (y < endOfSubItems - 1) {
                    mvwprintw(gParent->hWnd, hY + y, 4, "\u251C\u2500 %s", shortestTitle);
                }
                else {
                    mvwprintw(gParent->hWnd, hY + y, 4, "\u2514\u2500 %s", shortestTitle);
                }
            }
        }

        // After child items

        for (int y = firstSubItemY; y <= (getItemsCount() % hHeight - 1); y++) {
            ListItem* item = gListItems[y];
            if (item != NULL) {
                #ifdef _MSVC2005G
                    strcpy_s(shortestTitle, 288, item->title);
                #else
                    strcpy(shortestTitle, item->title);
                #endif
                if ((int)strlen(parentItem->title) > hWidth - 2) {
                    ExtString::strcut(shortestTitle, hWidth + 16, -1);
                }

                mvwprintw(gParent->hWnd, hY + y + subItemsCount, 4, "%s", shortestTitle);
            }
        }

        gItemCount += subItemsCount;
    }
    else if (!status && parentItem->expanded) {
        hSubItemCount = 0;
        for (int y = 0; y <= ((getItemsCount() - subItemsCount - 1) % hHeight); y++) {
            ListItem* item = gListItems[(gPageNumber * hHeight) + y];
            if (item != NULL) {
                #ifdef _MSVC2005G
                    strcpy_s(shortestTitle, 288, item->title);
                #else
                    strcpy(shortestTitle, item->title, 288);
                #endif
                if ((int)strlen(parentItem->title) > hWidth - 2) {
                    ExtString::strcut(shortestTitle, hWidth + 16, -1);
                }

                mvwprintw(gParent->hWnd, y + hY, 4, "%s", shortestTitle);
            }
        }
        gItemCount -= subItemsCount;
    }

    parentItem->expanded = status;
    expanded = status;
    wrefresh(gParent->hWnd);
    setSelectionIndex(pIndex2);
}

void ListBoxCtrl::drawListPointer(int x, int y, bool isVisible) {
    if (isVisible) {
        mvwaddch(gParent->hWnd, y, x, '*');  // Adds a star pointer to the specified position
        mvwchgat(gParent->hWnd, y, x, hWidth, A_BOLD, 1, NULL);
    }
    else {
        mvwaddch(gParent->hWnd, y, x, ' ');  // Removes a star pointer to the specified position
        mvwchgat(gParent->hWnd, y, x, hWidth, A_NORMAL, 2, NULL);
    }

    wrefresh(gParent->hWnd);
}

void ListBoxCtrl::drawListPointer(int x, int y, char p, bool isVisible) {
    if (isVisible) {
        mvwaddch(gParent->hWnd, y, x, p);  // Adds a star pointer to the specified position
        mvwchgat(gParent->hWnd, y, x, hWidth, A_BOLD, 1, NULL);
    }
    else {
        mvwaddch(gParent->hWnd, y, x, ' ');  // Removes a star pointer to the specified position
        mvwchgat(gParent->hWnd, y, x, hWidth, A_NORMAL, 2, NULL);
    }

    wrefresh(gParent->hWnd);
}
