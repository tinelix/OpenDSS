#include "extwnd.h"

ExtWindowCtrl::ExtWindowCtrl() {
    hCtrls = (UIControl**)malloc(sizeof(UIControl) * 255);
}

ExtWindowCtrl::~ExtWindowCtrl() {
    free(hCtrls);
}

void ExtWindowCtrl::addControl(UIControl* pCtrl) {
    hCtrls[getControlsSize() - 1] = pCtrl;
}

int ExtWindowCtrl::getControlsSize() {
    return sizeof(hCtrls) / sizeof(UIControl*);
}
