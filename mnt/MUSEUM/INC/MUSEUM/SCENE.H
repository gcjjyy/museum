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
    virtual void onLoad() = 0;
    virtual void onUnload() = 0;

   public:
    void draw();

   protected:
    Actor *getSceneRoot();

   private:
    Actor *m_sceneRoot;
};

#endif