#ifndef OPENDSS_WINDOWS_PLAYER_H
#define OPENDSS_WINDOWS_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#ifdef __MINGW64__
    #include <ncurses/ncurses.h>
#else
    #include <ncurses.h>
#endif
#include <cstdio>
#include <wchar.h>

#include "../controls/uictrl.h"
#include "../controls/extwnd.h"
#include "../controls/listbox.h"

#include "../utils/audtags.h"
#include "../utils/extstr.h"

#include "../decoders/audiodec.h"
#include "../decoders/mp3/mp3dec.h"

class AudioPlayerWnd : ExtWindowCtrl {
    public:
        AudioPlayerWnd(char* fname);
        ~AudioPlayerWnd();
        void onKeyPressed(char k);
    private:
        void prepare();
        void openAudioFile();
        void playAudioFile();
        void loadAudioTags();
        void drawVisualizer(int left, int right);
        char* getCodecName(int pCodecId);
        int gActiveWidth, gActiveHeight;
        char gFileName[384];
        AudioDecoder* gAudioDec;
        StreamInfo* gStreamInfo;
};

#endif
