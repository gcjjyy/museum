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
    uint16_t m_gridX;
    uint16_t m_gridY;
    uint16_t m_frameCount;
    uint16_t m_frameWidth;
    uint16_t m_frameHeight;
    uint16_t m_frameIndex;
    boolean_t m_useColorKey;
    uint8_t m_colorKey;
};

#endif
