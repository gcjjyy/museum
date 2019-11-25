#include <museum/director.h>
#include <stdio.h>

Director *Director::shared() {
    static Director inst;
    return &inst;
}

void Director::pushScene(Scene *scene) {
    vec_push(&this->m_scenes, (void *)scene);
    scene->onLoad();
}

Scene *Director::popScene() {
    if (this->m_scenes.length) {
        ((Scene *)vec_last(&this->m_scenes))->onUnload();
        return (Scene *)vec_pop(&this->m_scenes);
    } else {
        return NULL;
    }
}

Scene *Director::replaceScene(Scene *scene) {
    if (this->m_scenes.length) {
        Scene *prev = (Scene *)vec_pop(&this->m_scenes);
        vec_push(&this->m_scenes, scene);
        return prev;
    } else {
        return NULL;
    }
}

void Director::drawScene(float dt) {
    if (this->m_scenes.length) {
        Scene *current = (Scene *)vec_last(&this->m_scenes);
        current->draw(dt);
        current->onDraw(dt);
    }
}

Director::Director() {
    vec_init(&this->m_scenes);
}

Director::~Director() {
    vec_deinit(&this->m_scenes);
}
