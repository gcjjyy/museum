#include <stdio.h>
#include "common.h"
#include "director.h"
#include "museum.h"

Museum *Museum::m_shared = NULL;

Museum *Museum::shared() {
    if (!m_shared) {
        m_shared = new Museum();
    }

    return m_shared;
}

void Museum::run() {
    printf("run()\n");

    if (m_shared) {
        delete m_shared;
        m_shared = NULL;
    }
}

Museum::Museum() {
}

Museum::~Museum() {
}
