#ifndef __MUSEUM_H__
#define __MUSEUM_H__

#include <museum/actor.h>
#include <museum/director.h>
#include <museum/scene.h>
#include <museum/screen.h>
#include <museum/sprite.h>
#include <museum/text.h>

typedef enum {
    KEY_ESC,
    KEY_UP,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_ENTER,
    KEY_SPACE,
    KEY_L_CTRL,
    KEY_R_CTRL,
    KEY_L_ALT,
    KEY_R_ALT,
    KEY_L_SHIFT,
    KEY_R_SHIFT
} Key;

class Museum {
   public:
    static Museum *shared();

   public:
    void playMusic(const char *filename);
    void stopMusic();

    void init();
    void run();
    void quit();
    void deinit();

    boolean_t getKeyState(Key key);

   private:
    float getClock();

   private:
    Museum();
    ~Museum();

   private:
    boolean_t m_running;
};

#endif
