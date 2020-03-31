#include <amara.h>

using namespace Amara;
using namespace std;

int main(int argc, char** args) {
    Game* game = new Game("Amara Game");
    game->init(480, 360);

    // game->scenes->add("enterSceneKey", new GiveSceneInstance());
    // game->start("enterStartingScene");

    return 0;
}
