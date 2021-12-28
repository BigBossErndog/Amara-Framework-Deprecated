#include <amara.h>

using namespace Amara;
using namespace std;

// This is all test code and may not be necessarily helpful as an example.

class TestScene: public Scene {
    public:

        void preload() {
            
        }

        void create() {
            load->add("gradient", createGradient(
				this,
				640, 640,
				Up,
				{ 0, 0, 0, 255 },
				{ 0, 0, 0, 0 }
			));
			Image* img;
			add(img = new Image(0, 0, "gradient"));
        }

        void update() {

        }
};

int main(int argc, char** args) {
    #ifndef AMARA_WEB
        Game* game = new Game("Amara Game", false);
    #else
        WebGame* game = new WebGame("Amara Game");
    #endif
    game->init(480, 360);
    game->setWindowSize(960, 720);
    // game->setBackgroundColor(255, 255, 255);

    game->scenes->add("test", new TestScene());
    game->start("test");

    // game->scenes->add("enterSceneKey", new GiveSceneInstance());
    // game->start("enterStartingScene");

    return 0;
}
