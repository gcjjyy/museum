#include <museum/screen.h>
#include <museum/sprite.h>
#include <stdio.h>

Sprite::Sprite(const uint8_t *buf) {
    // Buffer format
    // - 2 bytes: width (Little endian)
    // - 2 bytes: height (Little endian)
    // - width * height bytes: Image data
    this->setSize(
        (buf[1] << 8) | (buf[0]),
        (buf[3] << 8) | (buf[2]));

    this->m_buf = buf;
}

Sprite::~Sprite() {
}

void Sprite::onDraw() {
    for (int i = 0; i < this->getHeight(); i++) {
        for (int j = 0; j < this->getWidth(); j++) {
            Screen::shared()->putPixel(j, i, this->m_buf[this->getWidth() * i + j]);
        }
    }
}
