#include "player.h"

class IOpenDSSAudioDecoder : IAudioDecoder {
    public:
        void onStreamClock(AudioSpectrum *spectrum, StreamTimestamp *streamTs);
        void onPlaybackStateChanged(int state);
        void setWindow(ExtWindowCtrl* pExtWnd);
};

AudioPlayerWnd::AudioPlayerWnd(char* fname) {

    sprintf(id, "msgBoxWnd");
    sprintf(hTitle, "%s", fname);
    sprintf(gFileName, "%s", fname);

    getmaxyx(stdscr, gActiveHeight, gActiveWidth);

    hWidth = gActiveWidth - 2;
    hHeight = gActiveHeight - 3;

    hWnd = newwin(hHeight, hWidth,
                  ((gActiveHeight - hHeight) / 2) + 1,
                  (gActiveWidth - hWidth) / 2);

    keypad(hWnd, true);

    box(hWnd, 0, 0);                            // <-- draw window borders
    mvwprintw(                                  // <-- draw window text in top border area
        hWnd,
        0, (gActiveWidth - wcslen((wchar_t*)hTitle) - 8) / 2.5,
        "\u2524 %s \u251c", hTitle
    );

    wbkgd(hWnd, COLOR_PAIR(2));

    wrefresh(hWnd);

    prepare();
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

    openAudioFile();
    playAudioFile();
}

void AudioPlayerWnd::loadAudioTags() {
    AudioTager *pTagger = new AudioTager();
    AudioTags *data = pTagger->readTags(gFileName);

    ExtWindowCtrl* fileInfoWnd = hChildWnds[2];

    if(data) {
        mvwprintw(
            fileInfoWnd->hWnd,
            2, 2,
            "Title:\t%s", data->title
        );
        mvwprintw(
            fileInfoWnd->hWnd,
            3, 2,
            "Artist:\t%s", data->artist
        );
        mvwprintw(
            fileInfoWnd->hWnd,
            4, 2,
            "Album:\t%s", data->album
        );
    }

    wrefresh(fileInfoWnd->hWnd);
}

void AudioPlayerWnd::openAudioFile() {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];
    ExtWindowCtrl* statsWnd = hChildWnds[2];

    IOpenDSSAudioDecoder* decInterface = new IOpenDSSAudioDecoder();
    decInterface->setWindow(this);

    gAudioDec = (AudioDecoder*) new MP3Decoder();
    gAudioDec->setInterface((IAudioDecoder*)decInterface);

    mvwprintw(
        playerCtrlWnd->hWnd,
        2, 2,
        "\u25BA Opening"
    );

    char* trackBar;
    int maxBlocksSize = (playerCtrlWnd->hWidth - 4);
    trackBar = (char*)malloc(maxBlocksSize * sizeof(char));

    wrefresh(playerCtrlWnd->hWnd);

    for(int i = 0; i < maxBlocksSize; i++) {
        mvwprintw(playerCtrlWnd->hWnd, 5, i + 2, "\u2500");
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

    if(gAudioDec->open(gFileName) == 0) {
        mvwprintw(
            playerCtrlWnd->hWnd,
            2, 2,
            "\u25BA Processing"
        );

        gStreamInfo = gAudioDec->getStreamInfo();
        mvwprintw(
            statsWnd->hWnd,
            6, 2,
            "%s", getCodecName(gStreamInfo->codec)
        );

        mvwchgat(statsWnd->hWnd, 6, 2, 10, A_BOLD, 6, NULL);

        mvwprintw(
            statsWnd->hWnd,
            6, 14,
            "%7d Hz", gStreamInfo->sampleRate
        );
        mvwchgat(statsWnd->hWnd, 6, 14, 10, A_BOLD, 6, NULL);

        mvwprintw(
            statsWnd->hWnd,
            6, 26,
            "%5d kbps", gStreamInfo->bitrate
        );
        mvwchgat(statsWnd->hWnd, 6, 26, 10, A_BOLD, 6, NULL);

        char* channelsIndicator = new char[10];

        sprintf(
            channelsIndicator,
            gStreamInfo->channels == 1 ? "Mono" : "Stereo"
        );

        mvwprintw(
            statsWnd->hWnd,
            6, 48 - strlen(channelsIndicator),
            "%s",
            channelsIndicator
        );
        mvwchgat(statsWnd->hWnd, 6, 38, 10, A_BOLD, 6, NULL);

        mvwprintw(
            playerCtrlWnd->hWnd,
            4, playerCtrlWnd->hWidth - 5 - 2,
            "%02d:%02d",
            gStreamInfo->lengthSec / 60, gStreamInfo->lengthSec % 60
        );
    } else {
        mvwprintw(
            playerCtrlWnd->hWnd,
            2, 2,
            "\u25BA Error #%d", gAudioDec->getErrorNumber()
        );
    }

    wrefresh(playerCtrlWnd->hWnd);
    wrefresh(statsWnd->hWnd);

    free(trackBar);
}

void AudioPlayerWnd::playAudioFile() {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];
    ExtWindowCtrl* statsWnd = hChildWnds[2];

    int maxBlocksSize = (playerCtrlWnd->hWidth - 4);

    for(int i = 0; i < maxBlocksSize; i++) {
        mvwaddch(playerCtrlWnd->hWnd, 2, 2 + i, ' ');
    }

    mvwprintw(
        playerCtrlWnd->hWnd,
        2, 2,
        "\u25BA Playing"
    );

    int status          = 0;
    int seconds         = 0;
    int minutes         = 0;

    if((status = gAudioDec->decode()) >= 0) {
        mvwprintw(
            playerCtrlWnd->hWnd,
            4, 2,
            "%02d:%02d",
            minutes, seconds % 60
        );
        wrefresh(playerCtrlWnd->hWnd);
    }

    wrefresh(playerCtrlWnd->hWnd);

    char k = wgetch(playlistWnd->hWnd);
    onKeyPressed(k);
}

void AudioPlayerWnd::onKeyPressed(char k) {
    ExtWindowCtrl* playerCtrlWnd = hChildWnds[0];
    ExtWindowCtrl* playlistWnd = hChildWnds[1];

    scrollok(playlistWnd->hWnd, TRUE);

    // ListBoxCtrl* categoriesListBox = ((ListBoxCtrl*) categoriesWnd->hCtrls[0]);
    // categoriesListBox->onKeyPressed(k);
    int seconds         = gAudioDec->getPlaybackPosition();
    int minutes         = 0;

    if(k != 'q') {
        if(k == (int)10) {
            mvwprintw(
                playerCtrlWnd->hWnd,
                4, 2,
                "%02d:%02d",
                minutes, seconds % 60
            );
        wrefresh(playerCtrlWnd->hWnd);
        }
        k = wgetch(playlistWnd->hWnd);
        onKeyPressed(k);
    } else {

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
        case STREAMINFO_CODEC_PCM_S16BE:
            sprintf(codecName, "PCM-S16BE");
            break;
        case STREAMINFO_CODEC_PCM_S16LE:
            sprintf(codecName, "PCM-S16LE");
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

    if(left / 100 > 1) {
        left = 100;
    }

    if(right / 100 > 1) {
        right = 100;
    }

    double onePerc = (double)(maxBlocksSize - 10) / 100;

    mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 3, 2, "L");
    for(int i = 0; i < maxBlocksSize; i++) {
        mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 3, i + 4, "\u25A0");
    }

    mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 2, 2, "R");
    for(int i = 0; i < maxBlocksSize; i++) {
        mvwprintw(statsWnd->hWnd, statsWnd->hHeight - 2, i + 4, "\u25A0");
    }

    mvwchgat(statsWnd->hWnd, statsWnd->hHeight - 3, 4, maxBlocksSize, A_BOLD, 9, NULL);
    mvwchgat(statsWnd->hWnd, statsWnd->hHeight - 3, 4, (left * onePerc), A_BOLD, 6, NULL);

    mvwchgat(statsWnd->hWnd, statsWnd->hHeight - 2, 4, maxBlocksSize, A_BOLD, 9, NULL);
    mvwchgat(statsWnd->hWnd, statsWnd->hHeight - 2, 4, (right * onePerc), A_BOLD, 6, NULL);

    wrefresh(statsWnd->hWnd);
}

AudioPlayerWnd::~AudioPlayerWnd() {
    wclear(hWnd);
    delwin(hWnd);
}

void IOpenDSSAudioDecoder::onStreamClock(
    AudioSpectrum *spectrum, StreamTimestamp *streamTs
) {
    ((AudioPlayerWnd*)hExtWnd)->drawVisualizer(spectrum->left, spectrum->right);
}

void IOpenDSSAudioDecoder::onPlaybackStateChanged(int state) {

}

void IOpenDSSAudioDecoder::setWindow(ExtWindowCtrl* pExtWnd) {
    hExtWnd = pExtWnd;
}
