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
    void init();
    void deinit();

    void loadGAT(int16_t num, const char *filename);
    void unloadGAT(int16_t num);
    void playGAT(int16_t num);
    void stopGAT();

    void loadWAV(int16_t num, const char *filename);
    void playWAV(int16_t num);

    void run();
    void quit();

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
