#include <museum/screen.h>
#include <museum/text.h>

Text::Text(const uint8_t *buf, FontType fontType) {
    this->m_buf = buf;
    this->m_fontType = fontType;
    this->m_fontColor = 0xff;
    this->m_text = NULL;
}

Text::~Text() {
    if (this->m_text) {
        delete [] this->m_text;
    }
}

Text *Text::setFontColor(uint8_t index) {
    this->m_fontColor = index;
    return this;
}

Text *Text::setText(const char *text) {
    if (this->m_text) {
        delete [] this->m_text;
        this->m_text = NULL;
    }

    if (text) {
        this->m_text = new char[strlen(text) + 1];
        strcpy(this->m_text, text);
    }

    return this;
}

void Text::drawFont8x16(char ch, int16_t *x, int16_t *y) {
    for (int i = 0; i < 16; i++) {
        uint8_t line = this->m_buf[(ch * 16) + i];
        for (int j = 0; j < 8; j++) {
            if (line & (0x80 >> j)) {
                Screen::shared()->putPixel((*x) + j, (*y) + i, this->m_fontColor);
            }
        }
    }

    *x = *x + 8;
}

void Text::onDraw() {
    int16_t x = this->getX();
    int16_t y = this->getY();

    for (int i = 0; i < strlen(this->m_text); i++) {
        switch (this->m_fontType) {
            case FONT_ENGLISH_8x16:
                this->drawFont8x16(this->m_text[i], &x, &y);
                break;
            default:
                break;
        }
    }
}
