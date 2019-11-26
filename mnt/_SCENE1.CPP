#include <museum/museum.h>
#include <stdio.h>
#include "_scene1.h"
#include "_scene2.h"
#include "asset/desert.h"
#include "asset/enemy_lg.h"
#include "asset/enemy_md.h"
#include "asset/enemy_sm.h"
#include "asset/eng8x16.h"
#include "asset/explo.h"
#include "asset/laser.h"
#include "asset/power_up.h"
#include "asset/ship.h"

void Scene1::onLoad() {
    this->m_bgY = 0.0f;

    this->m_bgBottom = this->getSceneRoot()->addActor(new Sprite(desert));
    this->m_bgBottom->setCoord(0, this->m_bgY);

    this->m_bgTop = this->getSceneRoot()->addActor(new Sprite(desert));
    this->m_bgTop->setCoord(0, this->m_bgY - this->m_bgTop->getHeight());

    this->m_player = this->getSceneRoot()->addActor(new Sprite(ship, 5, 2));
    this->m_player->setCoord(0, 128);

    this->getSceneRoot()->addActor((new Text(eng8x16, FONT_ENGLISH_8x16))->setText("Hello")->setFontColor(4))->setCoord(0, 128);

    this->getSceneRoot()->addActor(new Sprite(explo, 5, 1))->setCoord(0, 0);
    this->getSceneRoot()->addActor(new Sprite(laser, 2, 2))->setCoord(0, 32);
    this->getSceneRoot()->addActor(new Sprite(enemy_lg, 2, 1))->setCoord(160, 0);
    this->getSceneRoot()->addActor(new Sprite(enemy_md, 2, 1))->setCoord(160, 32);
    this->getSceneRoot()->addActor(new Sprite(enemy_sm, 2, 1))->setCoord(160, 64);
    this->getSceneRoot()->addActor(new Sprite(power_up, 2, 2))->setCoord(160, 96);

    Museum::shared()->loadWAV(0, "ASSET/1-UP.WAV");
    Museum::shared()->loadWAV(1, "ASSET/COIN.WAV");
    Museum::shared()->loadWAV(2, "ASSET/POWERUP.WAV");
    Museum::shared()->loadWAV(3, "ASSET/THROWING.WAV");

    Museum::shared()->loadGAT(0, "ASSET/SIDE-END.GAT");
    Museum::shared()->playGAT(0);
}

void Scene1::onUnload() {
}

void Scene1::onDraw(float dt) {
    if (Museum::shared()->getKeyState(KEY_UP)) {
        m_player->setCoord(
            m_player->getX(),
            m_player->getY() - 1);
    }

    if (Museum::shared()->getKeyState(KEY_LEFT)) {
        m_player->setCoord(
            m_player->getX() - 1,
            m_player->getY());
    }

    if (Museum::shared()->getKeyState(KEY_RIGHT)) {
        m_player->setCoord(
            m_player->getX() + 1,
            m_player->getY());
    }

    if (Museum::shared()->getKeyState(KEY_DOWN)) {
        m_player->setCoord(
            m_player->getX(),
            m_player->getY() + 1);
    }

    if (Museum::shared()->getKeyState(KEY_ESC)) {
        Museum::shared()->quit();
    }

    static int bb = 0;
    if (Museum::shared()->getKeyState(KEY_SPACE)) {
        if (!bb) bb = 1;
    } else if (bb) {
        bb = 0;
        Museum::shared()->playWAV(rand() % 4);
    }

    this->m_bgY += 0.01f;  // (dt * 16.0f);
    this->m_bgBottom->setCoord(0, m_bgY);
    this->m_bgTop->setCoord(0, m_bgY - this->m_bgTop->getHeight());
    if (this->m_bgY >= this->m_bgTop->getHeight()) {
        this->m_bgY -= this->m_bgTop->getHeight();
    }
}
