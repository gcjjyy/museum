#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

#include "stage.h"
#include "vec.h"

class Director {
   public:
    static Director *shared();

   public:
    void pushStage(Stage *stage);
    Stage *popStage();
    Stage *replaceStage(Stage *stage);

   private:
    Director();
    ~Director();

   private:
    vec_void_t m_stages;

   private:
    static Director *m_shared;
};

#endif
