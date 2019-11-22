#include <museum/director.h>
#include <museum/museum.h>

#include "_scene1.h"
#include "_scene2.h"
#include "_scene3.h"

int main(int argc, char *argv[]) {
    Museum::shared()->init();

    Scene1 *scene1 = new Scene1();
    Director::shared()->pushScene(scene1);

    Museum::shared()->run();

    delete Director::shared()->popScene();

    Museum::shared()->deinit();

    return 0;
}
