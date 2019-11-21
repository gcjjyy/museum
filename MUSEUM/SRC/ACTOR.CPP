#include <museum/actor.h>

Actor::Actor() {
}

Actor::~Actor() {
}

Actor *Actor::addActor(Actor *child) {
    vec_push(&this->m_child, (void *)child);
    return child;
}
