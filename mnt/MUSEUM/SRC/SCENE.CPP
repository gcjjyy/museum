#include <museum/scene.h>

Scene::Scene() {
    m_sceneRoot = new Actor();
}

Scene::~Scene() {
    delete m_sceneRoot;
}

Actor *Scene::getSceneRoot() {
    return this->m_sceneRoot;
}

void Scene::draw() {
    Actor *actor;
    vec_void_t queue;

    vec_init(&queue);
    vec_push(&queue, this->m_sceneRoot);

    while (queue.length) {
        actor = (Actor *)vec_pop(&queue);
        actor->onDraw();

        if (actor->m_child.length) {
            int iter = 0;
            void *child = NULL;

            // Push children via reversed order for BSP
            vec_foreach_rev(&actor->m_child, child, iter) {
                vec_push(&queue, child);
            }
        }
    }
}
