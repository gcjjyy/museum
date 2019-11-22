#include <museum/museum.h>
#include <stdio.h>
#include "_scene1.h"
#include "_scene2.h"
#include "rockman.h"

void Scene1::onLoad() {
    printf("Scene1 Created\n");
    this->getSceneRoot()->addActor(new Sprite(rockman));
    Museum::shared()->playMusic("BT-STRST.GAT");
}

void Scene1::onUnload() {
    printf("Scene1 Destroyed\n");
}

