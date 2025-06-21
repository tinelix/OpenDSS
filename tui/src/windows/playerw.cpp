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

#include "playerw.h"

int duplicateKeys = 0;
char prev_key;
bool disableListening;
int result;
bool dsePrepared;

#define MAX_FILE_LENGTH 80

class IOpenDSSAudioDecoder : ISoundEngineWrapper {
    public:
        void onStreamClock(DSE_AUDIO_SPECTRUM *spectrum, DSE_STREAM_TIMESTAMP *streamTs);
        void onPlaybackStateChanged(int state);
        void setWindow(ExtWindowCtrl* pExtWnd);
};


AudioPlayerWnd::AudioPlayerWnd(char* fname, ExtWindowCtrl* pParent, WINDOW* screen) {
    gParent = pParent;
    
    #ifdef _MSVC2005G
        sprintf_s(id, "msgBoxWnd");
    #else
        sprintf(id, "msgBoxWnd");
    #endif

    char* shortest_fname = new char[400];
    #ifdef _MSVC2005G
        sprintf_s(shortest_fname, 100, "%s", fname);
    #else
        sprintf(shortest_fname, "%s", fname);
    #endif
    if(strlen(shortest_fname) > MAX_FILE_LENGTH) {
        ExtString::strcut(shortest_fname, MAX_FILE_LENGTH - 3, -1);
        strcat(shortest_fname, "...");
    }

    #ifdef _MSVC2005G
        sprintf_s(hTitle, 100, "%s", shortest_fname);
    #else
        sprintf(hTitle, "%s", shortest_fname);
    #endif

    #ifdef _MSVC2005G
        sprintf_s(gFileName, 384, "%s", fname);
    #else
        sprintf(gFileName, "%s", fname);
    #endif

    hScreen = screen;

    getmaxyx(hScreen, gActiveHeight, gActiveWidth);

    hWidth = gActiveWidth - 2;
    hHeight = gActiveHeight - 3;

    hWnd = newwin(hHeight, hWidth,
                  ((gActiveHeight - hHeight) / 2) + 1,
                  (gActiveWidth - hWidth) / 2);

    keypad(hWnd, true);

    box(hWnd, 0, 0);                            // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        hWnd,
        0, (gActiveWidth - strlen(hTitle) - 4) / 2.5,
        "%c %s %c", 0xB4, hTitle, 0xC3
    );

    wbkgd(hWnd, COLOR_PAIR(2));

    wrefresh(hWnd);

    //set_escdelay(2);
    wtimeout(hWnd, 1);

    prepare();
    disableListening = false;
}

void AudioPlayerWnd::prepare() {

    hCtrls = (UIControl**)malloc(sizeof(UIControl) * 255);
    hChildWnds = (ExtWindowCtrl**)malloc(sizeof(ExtWindowCtrl) * 16);

    // Player control child window

    char playerCtrlWndId[] = "playerCtrlWndId";
    char playerCtrlWndTitle[] = "Audio Player";

    addChildWindow(playerCtrlWndId, playerCtrlWndTitle, 26, 7, 2, 3);

    // Playlist child window

    char playlistWndId[] = "playlistWndId";
    char playlistWndTitle[] = "Playlist";
    addChildWindow(playlistWndId, playlistWndTitle, 26, hHeight - 10, 2, 10);

    // Player control child window

    char statsWndId[] = "statsWndId";
    char statsWndTitle[] = "Statistics";

    addChildWindow(playerCtrlWndId, statsWndTitle, hWidth - 30, hHeight - 3, 28, 3);

    wrefresh(hWnd);

    drawVisualizer(0, 0);

    loadAudioTags();

	gWrapper = new SoundEngineWrapper();

    openAudioFile();
	if(result == 0)
		playAudioFile();
}

void AudioPlayerWnd::loadAudioTags() {
    // TODO: Add ID3v1/ID3v2/APE implementation
}

void AudioPlayerWnd::openAudioFile() {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];
    ExtWindowCtrl* statsWnd = hChildWnds[2];

    IOpenDSSAudioDecoder* decInterface = new IOpenDSSAudioDecoder();
    decInterface->setWindow(this);

    mvwprintw(
        playerCtrlWnd->hWnd,
        2, 2,
        "%c Opening",
		0xFE
    );

    char* trackBar;
    int maxBlocksSize = (playerCtrlWnd->hWidth - 4);
    trackBar = (char*)malloc(maxBlocksSize * sizeof(char));

    wrefresh(playerCtrlWnd->hWnd);

    for(int i = 0; i < maxBlocksSize; i++) {
        mvwprintw(playerCtrlWnd->hWnd, 5, i + 2, "%c", 0xC4);
    }

    mvwprintw(
        playerCtrlWnd->hWnd,
        4, 2,
        "%02d:%02d",
        0, 0
    );

    mvwprintw(
        playerCtrlWnd->hWnd,
        4, playerCtrlWnd->hWidth - 5 - 2,
        "%02d:%02d",
        0, 0
    );

	dsePrepared = gWrapper->init() >= 0;
	if(dsePrepared)
		result = gWrapper->openInputFile(gFileName);

	if(dsePrepared) {
        gStreamInfo = gWrapper->getStreamInfo();

        mvwprintw(
            statsWnd->hWnd,
            6, 2,
            "%s", getCodecName(gStreamInfo.codec_id)
        );

        mvwchgat(statsWnd->hWnd, 6, 2, 10, A_BOLD, 6, NULL);

        mvwprintw(
            statsWnd->hWnd,
            6, 12,
            "%7d Hz", gStreamInfo.sample_rate
        );
        mvwchgat(statsWnd->hWnd, 6, 14, 10, A_BOLD, 6, NULL);

        mvwprintw(
            statsWnd->hWnd,
            6, 24,
            "%5d kbps", gStreamInfo.bitrate / 1000
        );
        mvwchgat(statsWnd->hWnd, 6, 24, 10, A_BOLD, 6, NULL);

        char* channelsIndicator = new char[10];

        sprintf(
            channelsIndicator,
            gStreamInfo.channels == 1 ? "Mono" : "Stereo"
        );

        mvwprintw(
            statsWnd->hWnd,
            6, 46 - strlen(channelsIndicator),
            "%s",
            channelsIndicator
        );
        mvwchgat(statsWnd->hWnd, 6, 36, 10, A_BOLD, 6, NULL);

        /*mvwprintw(
            playerCtrlWnd->hWnd,
            4, playerCtrlWnd->hWidth - 5 - 2,
            "%02d:%02d",
            gStreamInfo->lengthSec / 60, gStreamInfo->lengthSec % 60
        );*/
    }

    wrefresh(playerCtrlWnd->hWnd);
    wrefresh(statsWnd->hWnd);

    free(trackBar);
}

void AudioPlayerWnd::playAudioFile() {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];
    ExtWindowCtrl* statsWnd = hChildWnds[2];

    disableListening = false;

    int maxBlocksSize = (playerCtrlWnd->hWidth - 4);

    for(int i = 0; i < maxBlocksSize; i++) {
        mvwaddch(playerCtrlWnd->hWnd, 2, 2 + i, ' ');
    }

    mvwprintw(
        playerCtrlWnd->hWnd,
        2, 2,
        "%c Playing",
		0xE2
    );

    int status          = 0;
    int seconds         = 0;
    int minutes         = 0;
	
	if(dsePrepared)
		gWrapper->play();

    wrefresh(playerCtrlWnd->hWnd);
}

void AudioPlayerWnd::onKeyPressed(char k) {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];
    ExtWindowCtrl* statsWnd = hChildWnds[2];

    scrollok(playlistWnd->hWnd, TRUE);

    //set_escdelay(2);
    wtimeout(hWnd, 1);

    // ListBoxCtrl* categoriesListBox = ((ListBoxCtrl*) categoriesWnd->hCtrls[0]);
    // categoriesListBox->onKeyPressed(k);
    if(k == (int)27) {
        char k2 = getch();
        if(k2 == -1) {
            freeWnd();
        }
    } else if(k >= 0 && k != 'q') {
        mvwprintw(hWnd, 1, 2, "Key: %d", k);
        wrefresh(hWnd);
    } else {
        duplicateKeys++;
    }
}

void AudioPlayerWnd::onKeyPressed(char k, char prev_k) {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];

    scrollok(playlistWnd->hWnd, TRUE);

    // ListBoxCtrl* categoriesListBox = ((ListBoxCtrl*) categoriesWnd->hCtrls[0]);
    // categoriesListBox->onKeyPressed(k);
    if(k == (int)27) {
        char k2 = wgetch(hWnd);
        if(k2 == -1) {
            disableListening = true;

            /*gAudioDec->stop();
            gAudioDec->freeStream();*/
        }
    } else if(k >= 0 && k != 'q') {
        mvwprintw(hWnd, 1, 2, "Key: %d", k);
        wrefresh(hWnd);
    } else {
        duplicateKeys++;
    }
}

char* AudioPlayerWnd::getCodecName(int pCodecId) {
    char *codecName = new char[20];
    switch(pCodecId) {
        case STREAMINFO_CODEC_PCM_U8:
            sprintf(codecName, "PCM-U8");
            break;
        case STREAMINFO_CODEC_PCM_S8:
            sprintf(codecName, "PCM-S8");
            break;
        case STREAMINFO_CODEC_PCM_S16LE:
            sprintf(codecName, "PCM-S16LE");
            break;
		case STREAMINFO_CODEC_PCM_S24LE:
            sprintf(codecName, "PCM-S24LE");
            break;
		case STREAMINFO_CODEC_PCM_S32LE:
            sprintf(codecName, "PCM-S32LE");
            break;
        case STREAMINFO_CODEC_FLAC_8:
            sprintf(codecName, "FLAC-8");
            break;
        case STREAMINFO_CODEC_FLAC_16:
            sprintf(codecName, "FLAC-16");
            break;
        case STREAMINFO_CODEC_FLAC_24:
            sprintf(codecName, "FLAC-24");
            break;
        case STREAMINFO_CODEC_MPEG1_L3:
            sprintf(codecName, "MPEG-1 L3");
            break;
        case STREAMINFO_CODEC_MPEG2_L3:
            sprintf(codecName, "MPEG-2 L3");
            break;
        case STREAMINFO_CODEC_MPEG2_L3_EXT:
            sprintf(codecName, "MPEG-2 L3+");
            break;
        case STREAMINFO_CODEC_AAC:
            sprintf(codecName, "LC-AAC");
            break;
        case STREAMINFO_CODEC_HE_AAC:
            sprintf(codecName, "HE-AAC");
            break;
        case STREAMINFO_CODEC_HE_AAC_V2:
            sprintf(codecName, "HE-AACv2");
            break;
        case STREAMINFO_CODEC_HE_AAC_V3:
            sprintf(codecName, "HE-AACv3");
            break;
        default:
            sprintf(codecName, "Unknown");
            break;
    }
    return codecName;
}

void AudioPlayerWnd::drawVisualizer(int left, int right) {
    ExtWindowCtrl* statsWnd = hChildWnds[2];
    int maxBlocksSize = (statsWnd->hWidth - 6);

    mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 3, 2, "L");
    for(int i = 0; i < maxBlocksSize; i++) {
        mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 3, i + 4, "%c", 0xFE);
    }

    mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 2, 2, "R");
    for(int i2 = 0; i2 < maxBlocksSize; i2++) {
        mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 2, i2 + 4, "%c", 0xFE);
    }

    // ColorPair 9 = COLOR_DARK_GRAY

	if(left > 100) {
		left = 100;
	}

	if(right > 100) {
		right = 100;
	}

    /*mvwchgat(statsWnd->hWnd, statsWnd->hHeight - 2, 4, maxBlocksSize, A_BOLD, 9, NULL);
    mvwchgat(
		statsWnd->hWnd, statsWnd->hHeight - 2, 4, 
		(double)((double)left / 100) * maxBlocksSize, A_BOLD, 6, NULL
	);

    mvwchgat(statsWnd->hWnd, statsWnd->hHeight - 2, 4, maxBlocksSize, A_BOLD, 9, NULL);
    mvwchgat(
		statsWnd->hWnd, statsWnd->hHeight - 2, 4, 
		(double)((double)right / 100) * maxBlocksSize, A_BOLD, 6, NULL
	);*/

    wrefresh(statsWnd->hWnd);
}

//void AudioPlayerWnd::updatePosition(StreamTimestamp *streamTs) {
//    int seconds         = streamTs->position;
//    int duration        = streamTs->duration;
//
//    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
//
//    int maxBlocksSize = (playerCtrlWnd->hWidth - 4);
//
//    int minutes = seconds / 60;
//
//    mvwprintw(
//        playerCtrlWnd->hWnd,
//        4, 2,
//        "%02d:%02d",
//        minutes, seconds % 60
//    );
//
//    int percents = ((float)seconds / duration) * maxBlocksSize;
//
//    for (int i = 0; i < maxBlocksSize; i++) {
//        if(i == percents) {
//            mvwprintw(playerCtrlWnd->hWnd, 5, i + 2, "\u25A0");
//        } else {
//            mvwprintw(playerCtrlWnd->hWnd, 5, i + 2, "\u2500");
//        }
//    }
//
//    wrefresh(playerCtrlWnd->hWnd);
//}

void AudioPlayerWnd::freeWnd() {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];
    ExtWindowCtrl* statsWnd = hChildWnds[2];

    delwin(playerCtrlWnd->hWnd);
    delwin(playlistWnd->hWnd);
    delwin(statsWnd->hWnd);
    delwin(hWnd);

    refresh();

    touchwin(gParent->hWnd);
    wrefresh(gParent->hWnd);
    gParent->listen(true);
    char k = wgetch(gParent->hWnd);
    gParent->onKeyPressed(k);
}

AudioPlayerWnd::~AudioPlayerWnd() {
    freeWnd();
}

//void IOpenDSSAudioDecoder::onStreamClock(
//    AudioSpectrum *spectrum, StreamTimestamp *streamTs
//) {
//    streamTs->position = ((AudioPlayerWnd*)hExtWnd)->gAudioDec->getPlaybackPosition();
//    streamTs->duration = ((AudioPlayerWnd*)hExtWnd)->gAudioDec->getPlaybackDuration();
//    ((AudioPlayerWnd*)hExtWnd)->updatePosition(streamTs);
//    ((AudioPlayerWnd*)hExtWnd)->drawVisualizer(spectrum->left, spectrum->right);
//    char k = wgetch(hExtWnd->hWnd);
//    if(!disableListening) {
//        if(k != prev_key)
//            ((AudioPlayerWnd*)hExtWnd)->onKeyPressed(k, prev_key);
//        prev_key = k;
//    }
//}

void ISoundEngineWrapper::onStreamClock(DSE_AUDIO_SPECTRUM *spectrum, DSE_STREAM_TIMESTAMP *streamTs) {

}

void ISoundEngineWrapper::onPlaybackStateChanged(int state) {
	
}

void ISoundEngineWrapper::setWindow(ExtWindowCtrl* pExtWnd) {
    hExtWnd = pExtWnd;
}
