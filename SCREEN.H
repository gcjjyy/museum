#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "COMMON.H"

class Screen {
   public:
    Screen();
    ~Screen();

   public:
    void setMode(uint8_t mode);
    void setPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    int setPaletteFromFile(const char *filename);

    void putPixel(int16_t x, int16_t y, uint8_t index);

   private:
    uint32_t m_scanLinePos[SCREEN_HEIGHT];
};

#endif
