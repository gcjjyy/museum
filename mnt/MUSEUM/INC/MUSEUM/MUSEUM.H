#ifndef __MUSEUM_H__
#define __MUSEUM_H__

#include <museum/actor.h>
#include <museum/director.h>
#include <museum/scene.h>
#include <museum/screen.h>
#include <museum/sprite.h>

class Museum {
   public:
    static Museum *shared();

   public:
    void playMusic(const char *filename);
    void stopMusic();

    void init();
    void run();
    void deinit();

   private:
    Museum();
    ~Museum();

   private:
    boolean_t m_running;
};

#endif
