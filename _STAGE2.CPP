#include <stdio.h>
#include <director.h>
#include "_stage2.h"

void Stage2::onLoad() {
    printf("Stage2 Created\n");
    Director::shared()->popStage();
}

void Stage2::onUnload() {
    printf("Stage2 Destroyed\n");
}
