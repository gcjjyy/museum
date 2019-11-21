#include <stdio.h>
#include <conio.h>
#include <museum/common.h>
#include <museum/director.h>
#include <museum/museum.h>
#include <museum/screen.h>

Museum *Museum::shared() {
    static Museum inst;
    return &inst;
}

void Museum::run() {
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
}

Museum::Museum() {
}

Museum::~Museum() {
}
