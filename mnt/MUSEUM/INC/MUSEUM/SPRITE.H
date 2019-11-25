#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <museum/actor.h>

class Sprite : public Actor {
   public:
    Sprite(const uint8_t *buf, uint16_t gridX = 1, uint16_t gridY = 1);
    virtual ~Sprite();

   public:
    void enableColorKey(uint8_t index);
    void disableColorKey();

   protected:
    virtual void onDraw();

   private:
    const uint8_t *m_buf;
    int16_t m_gridX;
    int16_t m_gridY;
    int16_t m_frameCount;
    int16_t m_frameWidth;
    int16_t m_frameHeight;
    int16_t m_frameIndex;
    int16_t m_imageWidth;
    int16_t m_imageHeight;
    boolean_t m_useColorKey;
    uint8_t m_colorKey;
};

#endif
