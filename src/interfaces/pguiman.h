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
