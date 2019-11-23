#include <museum/screen.h>
#include <museum/sprite.h>

Sprite::Sprite(const uint8_t *buf, uint16_t gridX, uint16_t gridY) {
    // Buffer format
    // - 2 bytes: width (Little endian)
    // - 2 bytes: height (Little endian)
    // - width * height bytes: Image data
    this->setSize(
        (buf[1] << 8) | (buf[0]),
        (buf[3] << 8) | (buf[2]));

    if (buf[4] == 0x01) {
        this->enableColorKey(0xff);
    } else {
        this->disableColorKey();
    }

    this->m_buf = (buf + 5);

    this->m_gridX = gridX;
    this->m_gridY = gridY;
    this->m_frameCount = gridX * gridY;
    this->m_frameIndex = 0;

    this->m_frameWidth = this->getWidth() / gridX;
    this->m_frameHeight = this->getHeight() / gridY;
}

Sprite::~Sprite() {
}

void Sprite::enableColorKey(uint8_t index) {
    this->m_useColorKey = TRUE;
    this->m_colorKey = index;
}

void Sprite::disableColorKey() {
    this->m_useColorKey = FALSE;
}

void Sprite::onDraw() {
    uint16_t frameX = (this->m_frameIndex % this->m_gridX) * this->m_frameWidth;
    uint16_t frameY = (this->m_frameIndex / this->m_gridX) * this->m_frameHeight;

    for (int16_t i = 0; i < m_frameHeight; i++) {
        for (int16_t j = 0; j < m_frameWidth; j++) {
            uint8_t index = this->m_buf[this->getWidth() * (frameY + i) + (frameX + j)];
            if (!this->m_useColorKey || index != 0xff) {
                Screen::shared()->putPixel(this->getX() + j, this->getY() + i, index);
            }
        }
    }

    this->m_frameIndex = (this->m_frameIndex + 1) % this->m_frameCount;
}
