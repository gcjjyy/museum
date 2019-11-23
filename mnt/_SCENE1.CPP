#include <museum/museum.h>
#include <stdio.h>
#include "_scene1.h"
#include "_scene2.h"

#include "asset/desert.h"
#include "asset/enemy_lg.h"
#include "asset/enemy_md.h"
#include "asset/enemy_sm.h"
#include "asset/explo.h"
#include "asset/laser.h"
#include "asset/power_up.h"
#include "asset/ship.h"
#include "asset/eng8x16.h"

void Scene1::onLoad() {
    this->getSceneRoot()->addActor(new Sprite(desert))->setCoord(0, 0);
    this->getSceneRoot()->addActor(new Sprite(explo, 5, 1))->setCoord(0, 0);
    this->getSceneRoot()->addActor(new Sprite(laser, 2, 2))->setCoord(0, 32);
    this->getSceneRoot()->addActor(new Sprite(ship, 5, 2))->setCoord(0, 64);

    this->getSceneRoot()->addActor(new Sprite(enemy_lg, 2, 1))->setCoord(160, 0);
    this->getSceneRoot()->addActor(new Sprite(enemy_md, 2, 1))->setCoord(160, 32);
    this->getSceneRoot()->addActor(new Sprite(enemy_sm, 2, 1))->setCoord(160, 64);
    this->getSceneRoot()->addActor(new Sprite(power_up, 2, 2))->setCoord(160, 96);

    ((Text *)this->getSceneRoot()->addActor(new Text(eng8x16, FONT_ENGLISH_8x16))->setCoord(0, 128))->setText("Hello")->setFontColor(4);

    Museum::shared()->playMusic("MUSIC/BT-STRST.GAT");
}

void Scene1::onUnload() {
}
