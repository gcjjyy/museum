#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <museum/actor.h>

class Sprite : public Actor {
   public:
    Sprite(const uint8_t *buf);
    virtual ~Sprite();

   protected:
    virtual void onDraw();

   private:
    const uint8_t *m_buf;
};

#endif
