#include <stdio.h>
#include "sprite.h"

Sprite::Sprite(uint8_t *buf) {
}

Sprite::~Sprite() {
}

void Sprite::onDraw() {
    printf("Sprite::onDraw...\n");
}
