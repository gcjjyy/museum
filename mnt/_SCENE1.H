#include <museum/scene.h>

class Scene1 : public Scene {
   public:
    virtual void onLoad();
    virtual void onUnload();
    virtual void onDraw();

   private:
    Actor *m_player;
};
