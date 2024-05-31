#include "audiodec.h"

IAudioDecoder::IAudioDecoder() {

}

void IAudioDecoder::setWindow(ExtWindowCtrl* pExtWnd) {
    hExtWnd = pExtWnd;
}
