/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  -------------------------------------------------------------------------------------------
 *  Copyright © 2024 Dmitry Tretyakov (aka. Tinelix)
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of
 *  the GNU General Public License 3 (or any later version) and/or Apache License 2
 *  See the following files in repository directory for the precise terms and conditions of
 *  either license:
 *
 *     LICENSE.APACHE
 *     LICENSE.GPLv3
 *
 *  Please see each file in the implementation for copyright and licensing information,
 *  (in the opening comment of each file).
 */

#ifndef OPENDSS_CONTROLS_LISTBOX_H
#define OPENDSS_CONTROLS_LISTBOX_H

#include "extwnd.h"
#include "uictrl.h"
#include "../utils/extstr.h"
#include <cstring>
#include <cstdio>

#include <curses.h>

struct ListItem {
    char title[384];
    void* ptr;
    struct ListItem** subItems;
    int subItemsCount;
    bool expanded;
    bool subItem;
};

class ListBoxCtrl : UIControl {
    public:
        ListBoxCtrl(ExtWindowCtrl* pParent, int pItemCount, bool pTrackPos);
        ~ListBoxCtrl();
        void addListItem(int index, ListItem* item);
        int getSelectionIndex();
        void setSelectionIndex(int index);
        void onKeyPressed(char k);
        void drawListPointer(int x, int y, bool isVisible);
        void drawListPointer(int x, int y, char p, bool isVisible);
        void goToPage(int pPageNumber);
        void recreate(int pItemCount);
        void expand(int pIndex, bool status);
        struct ListItem** getItems();
        int getPageNumber();
        int getItemsCount();
        int getSubItemsCount(int pIndex);
        int hX, hY, hWidth, hHeight, hExpandedItemIndex, hSubItemCount;
        bool expanded;
    protected:
        struct ListItem**   gListItems;
        bool gTrackPos;
        int getVirtualSelectionIndex();
        ExtWindowCtrl* gParent;
        int gSelectionIndex, gPageNumber, gItemCount;
};

#endif
