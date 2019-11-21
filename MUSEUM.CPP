#include <stdio.h>
#include <conio.h>
#include "common.h"
#include "director.h"
#include "museum.h"
#include "screen.h"

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
