#ifndef __SCENE_H__
#define __SCENE_H__

#include <museum/actor.h>
#include <museum/common.h>
#include <vec/vec.h>

class Scene {
   public:
    Scene();
    virtual ~Scene();

   public:
    virtual void onLoad() {}
    virtual void onUnload() {}
    virtual void onDraw(float dt) {}

   public:
    void draw(float dt);

   protected:
    Actor *getSceneRoot();

   private:
    Actor *m_sceneRoot;
};

#endif
