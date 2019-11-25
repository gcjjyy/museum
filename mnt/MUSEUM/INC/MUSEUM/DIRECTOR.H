#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include <museum/scene.h>
#include <vec/vec.h>

class Director {
   public:
    static Director *shared();

   public:
    void pushScene(Scene *scene);
    Scene *popScene();
    Scene *replaceScene(Scene *scene);
    void drawScene(float dt);

   private:
    Director();
    ~Director();

   private:
    vec_void_t m_scenes;
};

#endif
