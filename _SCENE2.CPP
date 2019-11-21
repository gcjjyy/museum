#include <stdio.h>
#include <director.h>
#include "_scene2.h"

void Scene2::onLoad() {
    printf("Scene2 Created\n");
    Director::shared()->popScene();
}

void Scene2::onUnload() {
    printf("Scene2 Destroyed\n");
}
