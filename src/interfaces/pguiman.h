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

#ifndef OPENDSS_INTERFACES_PGUIMAN_H
#define OPENDSS_INTERFACES_PGUIMAN_H

#include "../controls/extwnd.h"

class IPseudoGUIManager {
    public:
        IPseudoGUIManager() {};
        virtual ~IPseudoGUIManager() {};
        virtual void onKeyPressed(char k) = 0;
        virtual void onKeyPressed(char k, ExtWindowCtrl* pExtWnd) = 0;
};

#endif // OPENDSS_INTERFACES_PGUIMAN_H
