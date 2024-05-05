#ifndef OPENDSS_INTERFACES_PGUIMAN_H
#define OPENDSS_INTERFACES_PGUIMAN_H

class IPseudoGUIManager {
    public:
        IPseudoGUIManager() {};
        virtual ~IPseudoGUIManager() {};
        virtual void onKeyPressed(char k) = 0;
};

#endif // OPENDSS_INTERFACES_PGUIMAN_H
