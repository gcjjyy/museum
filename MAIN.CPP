#include <director.h>
#include <museum.h>

#include "_stage1.h"
#include "_stage2.h"
#include "_stage3.h"

int main(int argc, char *argv[]) {
    Stage1 *stage1 = new Stage1();

    Director::shared()->pushStage(stage1);

    Museum::shared()->run();

    delete Director::shared()->popStage();

    return 0;
}
