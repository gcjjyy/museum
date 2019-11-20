#include <stdio.h>
#include "_stage1.h"

void Stage1::onLoad() {
    printf("Stage1 Created\n");
}

void Stage1::onUnload() {
    printf("Stage1 Destroyed\n");
}
