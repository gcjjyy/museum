#include <stdio.h>
#include <director.h>
#include "_stage1.h"
#include "_stage2.h"

void Stage1::onLoad() {
    printf("Stage1 Created\n");
    Director::shared()->pushStage(new Stage2());
}

void Stage1::onUnload() {
    printf("Stage1 Destroyed\n");
}
