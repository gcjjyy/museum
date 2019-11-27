#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <museum/common.h>

class Screen {
   public:
    static Screen *shared();

   public:
    void setMode(uint8_t mode);

    void clear();
    void setPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    int setPaletteFromFile(const char *filename);

    void drawBitmap(int16_t x, int16_t y, const uint8_t *buf, int16_t cropX,
                    int16_t cropY, int16_t cropWidth, int16_t cropHeight,
                    int16_t imageWidth, int16_t imageHeight);
    void putPixel(int16_t x, int16_t y, uint8_t index);

    void update();

   private:
    Screen();
    ~Screen();

   private:
    uint32_t m_scanLinePos[SCREEN_HEIGHT];
    uint8_t *m_fb;
};

#endif
