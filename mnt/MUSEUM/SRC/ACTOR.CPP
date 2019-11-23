#include <museum/actor.h>

Actor::Actor() {
}

Actor::~Actor() {
}

Actor *Actor::setCoord(int16_t x, int16_t y) {
    this->m_x = x;
    this->m_y = y;

    return this;
}

Actor *Actor::setSize(int16_t width, int16_t height) {
    this->m_width = width;
    this->m_height = height;

    return this;
}

int16_t Actor::getX() {
    return this->m_x;
}

int16_t Actor::getY() {
    return this->m_y;
}

int16_t Actor::getWidth() {
    return this->m_width;
}

int16_t Actor::getHeight() {
    return this->m_height;
}

Actor *Actor::addActor(Actor *child) {
    vec_push(&this->m_child, (void *)child);
    return child;
}
