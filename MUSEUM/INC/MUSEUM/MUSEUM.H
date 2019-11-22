#ifndef __MUSEUM_H__
#define __MUSEUM_H__

#include <museum/director.h>
#include <museum/scene.h>
#include <museum/actor.h>
#include <museum/sprite.h>

class Museum {
   public:
    static Museum *shared();

   public:
    void playMusic(const char *filename);
    void stopMusic();

    void run();

   private:
    Museum();
    ~Museum();

   private:
    boolean_t m_running;
};

#endif
