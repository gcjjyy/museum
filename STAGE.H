#ifndef __STAGE_H__
#define __STAGE_H__

#include "common.h"

class Stage {
   public:
    Stage();
    virtual ~Stage();

   public:
    virtual void onLoad() = 0;
    virtual void onUnload() = 0;
};

#endif
