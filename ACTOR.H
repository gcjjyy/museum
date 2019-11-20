#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "vec.h"

class Actor {
   public:
    Actor();
    virtual ~Actor();

   public:
    void AddChild(Actor *actor);

   private:
    vec_void_t m_child;
};

#endif
