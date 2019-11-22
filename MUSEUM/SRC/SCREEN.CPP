#include <conio.h>
#include <i86.h>
#include <museum/screen.h>
#include <stdio.h>
#include <memory.h>

#define VRAM ((uint8_t far *)0xa0000)

Screen *Screen::shared() {
    static Screen inst;
    return &inst;
}

void Screen::setMode(uint8_t mode) {
    union REGS r;
    r.x.eax = mode;
    int386(0x10, &r, &r);

    if (mode == 0x13) {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            m_scanLinePos[i] = SCREEN_WIDTH * i;
        }
    }
}

void Screen::setPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    outp(0x03c8, index);
    outp(0x03c9, r >> 2);
    outp(0x03c9, g >> 2);
    outp(0x03c9, b >> 2);
}

int Screen::setPaletteFromFile(const char *filename) {
    uint8_t pal[768];
    FILE *fp;

    fp = fopen(filename, "rb");
    if (!fp) {
        return MUSM_ERR;
    }

    if (fread(pal, 1, 768, fp) != 768) {
        fclose(fp);
        return MUSM_ERR;
    }

    outp(0x03c8, 0);
    for (int i = 0; i < 768; i++) {
        outp(0x03c9, pal[i] >> 2);
    }

    return MUSM_OK;
}

void Screen::putPixel(int16_t x, int16_t y, uint8_t index) {
    if (x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
        m_fb[m_scanLinePos[y] + x] = index;
    }
}

void Screen::update() {
    memcpy((void far *)VRAM, (void *)m_fb, SCREEN_WIDTH * SCREEN_HEIGHT);
}

Screen::Screen() {
    m_fb = new uint8_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    memset((void *)m_fb, 0x00, SCREEN_WIDTH * SCREEN_HEIGHT);
}

Screen::~Screen() {
    delete [] m_fb;
}
