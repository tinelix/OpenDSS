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

#include "../interfaces/audiodec.h"

#include "../utils/audtags.h"
#include "../utils/extstr.h"

#include "../decoders/audiodec.h"
#include "../decoders/mp3/mp3dec.h"

class AudioPlayerWnd : ExtWindowCtrl {
    public:
        AudioPlayerWnd(char* fname, ExtWindowCtrl* pParent);
        ~AudioPlayerWnd();
        void onKeyPressed(char k);
        void onKeyPressed(char k, char prev_k);
        void drawVisualizer(int left, int right);
        void updatePosition(StreamTimestamp *streamTs);
        void freeWnd();
        AudioDecoder* gAudioDec;
    private:
        void prepare();
        void openAudioFile();
        void playAudioFile();
        void loadAudioTags();
        char* getCodecName(int pCodecId);
        int gActiveWidth, gActiveHeight;
        char gFileName[384];
        StreamInfo* gStreamInfo;
};

#endif
