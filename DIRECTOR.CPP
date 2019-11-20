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
    stage->onLoad();
}

Stage *Director::popStage() {
    return NULL;
}

Stage *Director::replaceStage(Stage *stage) {
    return NULL;
}

Director::Director() {
}

Director::~Director() {
}
