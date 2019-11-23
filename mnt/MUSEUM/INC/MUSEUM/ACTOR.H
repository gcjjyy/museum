#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <museum/common.h>
#include <vec/vec.h>

class Scene;

class Actor {
    friend class Scene;

   public:
    Actor();
    virtual ~Actor();

   public:
    Actor *setCoord(int16_t x, int16_t y);
    Actor *setSize(int16_t width, int16_t height);

    int16_t getX();
    int16_t getY();
    int16_t getWidth();
    int16_t getHeight();

    Actor *addActor(Actor *actor);

   protected:
    virtual void onDraw() { /* Abstract */
    }

   private:
    int16_t m_x;
    int16_t m_y;
    int16_t m_width;
    int16_t m_height;
    vec_void_t m_child;
};

#endif
