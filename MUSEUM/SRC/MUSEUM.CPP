#include <stdio.h>
#include <conio.h>
#include <museum/common.h>
#include <museum/director.h>
#include <museum/museum.h>
#include <museum/screen.h>
#include <gat_wc/gat_wc.h>

#define GAT_MUSIC_NO (0)

Museum *Museum::shared() {
    static Museum inst;
    return &inst;
}

void Museum::run() {
    ::gatInit();
    ::gatSetVol(100);

    Screen::shared()->setMode(0x13);

    m_running = TRUE;
    while (m_running) {
        Director::shared()->drawScene();

        if (kbhit()) {
            char ch = getch();
            if (ch == 27) {
                break;
            }
        }
    }

    Screen::shared()->setMode(0x03);

    ::gatClose();
}

void Museum::playMusic(const char *filename) {
    if (gatLoad(GAT_MUSIC_NO, (unsigned char *)filename)) {
        gatPlay(GAT_MUSIC_NO);
    }
}

void Museum::stopMusic() {
    gatStop();
    gatFree(GAT_MUSIC_NO);
}

Museum::Museum() {
}

Museum::~Museum() {
}
