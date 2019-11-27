#include <conio.h>
#include <i86.h>
#include <memory.h>
#include <museum/screen.h>
#include <stdio.h>

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

void Screen::clear() { memset(m_fb, 0, SCREEN_WIDTH * SCREEN_HEIGHT); }

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

/**
 * @brief Draw a bitmap buffer via memcpy (Doesn't support colorkey)
 */
void Screen::drawBitmap(int16_t x, int16_t y, const uint8_t *buf, int16_t cropX,
                        int16_t cropY, int16_t cropWidth, int16_t cropHeight,
                        int16_t imageWidth, int16_t imageHeight) {
    int16_t clipX = x;
    int16_t clipY = y;
    int16_t clipWidth = cropWidth;
    int16_t clipHeight = cropHeight;
    int16_t offsetX = cropX;
    int16_t offsetY = cropY;

    if (clipX < 0) {
        clipWidth += clipX;
        offsetX -= clipX;
        clipX = 0;
    }

    if (clipY < 0) {
        clipHeight += clipY;
        offsetY -= clipY;
        clipY = 0;
    }

    if ((clipX + clipWidth) > SCREEN_WIDTH) {
        clipWidth -= ((clipX + clipWidth) - SCREEN_WIDTH);
    }

    if ((clipY + clipHeight) > SCREEN_HEIGHT) {
        clipHeight -= ((clipY + clipHeight) - SCREEN_HEIGHT);
    }

    for (int16_t i = 0; i < clipHeight; i++) {
        memcpy(&m_fb[m_scanLinePos[i + clipY] + clipX],
               &buf[imageWidth * (i + offsetY) + offsetX], clipWidth);
    }
}

void Screen::putPixel(int16_t x, int16_t y, uint8_t index) {
    if (x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
        m_fb[m_scanLinePos[y] + x] = index;
    }
}

void Screen::update() {
    memcpy((void *)VRAM, (void *)m_fb, SCREEN_WIDTH * SCREEN_HEIGHT);
}

Screen::Screen() {
    m_fb = new uint8_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    memset((void *)m_fb, 0x00, SCREEN_WIDTH * SCREEN_HEIGHT);
}

Screen::~Screen() { delete[] m_fb; }
