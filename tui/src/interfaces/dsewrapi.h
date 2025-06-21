/*  Tinelix OpenDSS - open sourced clone of Digital Sound System player
 *  ----------------------------------------------------------------------------
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
 *  information, (in the opening comment of each file).
 */

#pragma once

#include <controls/extwnd.h>

typedef struct DSE_STREAM_TIMESTAMP {
    int position, duration;
};

typedef struct DSE_AUDIO_SPECTRUM {
    int left, right;
    int channels;
};

class ISoundEngineWrapper {
    public:
        ISoundEngineWrapper();
        ~ISoundEngineWrapper();
        void setWindow(ExtWindowCtrl* pExtWnd);
		void onPlaybackStateChanged(int state) {};
		void onStreamClock(DSE_AUDIO_SPECTRUM spectrum, DSE_STREAM_TIMESTAMP streamTs) {};
    protected:
        ExtWindowCtrl* hExtWnd;
};
