#include <museum/scene.h>

class Scene1 : public Scene {
   public:
    virtual void onLoad();
    virtual void onUnload();
    virtual void onDraw(float dt);

   private:
    Actor *m_player;
    Actor *m_bgTop;
    Actor *m_bgBottom;
    float m_bgY;
};
