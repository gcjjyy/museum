#ifndef __TEXT_H__
#define __TEXT_H__

#include <museum/actor.h>

typedef enum {
    FONT_ENGLISH_8x16
} FontType;

class Text : public Actor {
   public:
    Text(const uint8_t *buf, FontType fontType);
    virtual ~Text();

   public:
    Text *setFontColor(uint8_t index);
    Text *setText(const char *text);

   protected:
    virtual void onDraw(float dt);

   private:
    void drawFont8x16(char ch, int16_t *x, int16_t *y);

   private:
    const uint8_t *m_buf;
    FontType m_fontType;
    uint8_t m_fontColor;
    char *m_text;
};

#endif
