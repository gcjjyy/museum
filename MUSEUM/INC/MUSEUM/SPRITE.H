#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <museum/actor.h>

class Sprite : public Actor {
   public:
    Sprite(uint8_t *buf);
    virtual ~Sprite();

   protected:
    virtual void onDraw();

   private:
};

#endif
