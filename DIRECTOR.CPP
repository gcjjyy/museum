#include <stdio.h>
#include "director.h"

Director *Director::m_shared = NULL;

Director *Director::shared() {
    if (!m_shared) {
        m_shared = new Director();
    }

    return m_shared;
}

void Director::pushStage(Stage *stage) {
    vec_push(&this->m_stages, (void *)stage);
    stage->onLoad();
}

Stage *Director::popStage() {
    if (this->m_stages.length > 0) {
        ((Stage *)vec_last(&this->m_stages))->onUnload();
        return (Stage *)vec_pop(&this->m_stages);
    } else {
        return NULL;
    }
}

Stage *Director::replaceStage(Stage *stage) {
    if (this->m_stages.length > 0) {
        Stage *prev = (Stage *)vec_pop(&this->m_stages);
        vec_push(&this->m_stages, stage);
        return prev;
    } else {
        return NULL;
    }
}

Director::Director() {
    vec_init(&this->m_stages);
}

Director::~Director() {
    vec_deinit(&this->m_stages);
}
