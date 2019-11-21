#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include "scene.h"
#include "vec.h"

class Director {
   public:
    static Director *shared();

   public:
    void pushScene(Scene *scene);
    Scene *popScene();
    Scene *replaceScene(Scene *scene);
    void drawScene();

   private:
    Director();
    ~Director();

   private:
    vec_void_t m_scenes;
};

#endif
