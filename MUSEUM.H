#ifndef __MUSEUM_H__
#define __MUSEUM_H__

#include "director.h"
#include "scene.h"
#include "actor.h"
#include "sprite.h"

class Museum {
   public:
    static Museum *shared();

   public:
    void run();

   private:
    Museum();
    ~Museum();

   private:
    boolean_t m_running;
};

#endif
