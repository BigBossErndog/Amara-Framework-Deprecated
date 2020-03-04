#include <amara.h>
using namespace Amara;

int main(int argc, char** args) {
    Game* game = new Game("Amara Test Build");
    game->init(480, 360);
    game->start("Fun");
}