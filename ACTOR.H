#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "common.h"
#include "vec.h"

class Scene;

class Actor {
    friend class Scene;

   public:
    Actor();
    virtual ~Actor();

   public:
    Actor *addActor(Actor *actor);

   protected:
    virtual void onDraw() { /* Abstract */ }

   private:
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    vec_void_t m_child;
};

#endif
