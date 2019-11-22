#include <museum/museum.h>
#include <stdio.h>
#include "_scene1.h"
#include "_scene2.h"

#include "asset/desert.h"
//#include "asset/enemy_lg.h"
//#include "asset/enemy_md.h"
//#include "asset/enemy_sm.h"
#include "asset/explo.h"
#include "asset/laser.h"
//#include "asset/power_up.h"
#include "asset/ship.h"

void Scene1::onLoad() {
    this->getSceneRoot()->addActor(new Sprite(desert))->setCoord(0, 0);
    this->getSceneRoot()->addActor(new Sprite(explo))->setCoord(0, 0);
    this->getSceneRoot()->addActor(new Sprite(laser))->setCoord(0, 32);
    this->getSceneRoot()->addActor(new Sprite(ship))->setCoord(0, 64);
    // Museum::shared()->playMusic("MUSIC/BT-STRST.GAT");
}

void Scene1::onUnload() {
}
