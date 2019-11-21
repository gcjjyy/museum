#include <stdio.h>
#include <museum.h>
#include "_scene1.h"
#include "_scene2.h"

void Scene1::onLoad() {
    printf("Scene1 Created\n");

    this->getSceneRoot()->addActor(new Sprite(NULL));
}

void Scene1::onUnload() {
    printf("Scene1 Destroyed\n");
}
