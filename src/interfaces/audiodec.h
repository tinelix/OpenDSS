#ifndef OPENDSS_INTERFACES_AUDIODEC_H
#define OPENDSS_INTERFACES_AUDIODEC_H

#include "../decoders/stream.h"
#include "../controls/extwnd.h"

class IAudioDecoder {
    public:
        IAudioDecoder();
        void setWindow(ExtWindowCtrl* hExtWnd);
        virtual void onStreamClock(AudioSpectrum *spectrum, StreamTimestamp *streamTs) = 0;
        virtual void onPlaybackStateChanged(int state) = 0;
    protected:
        ExtWindowCtrl* hExtWnd;
};

#endif
