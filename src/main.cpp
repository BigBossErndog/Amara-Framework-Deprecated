#include <amara.h>

using namespace Amara;
using namespace std;

// This is all test code and may not be necessarily helpful as an example.

class TestScene: public Scene {
    public:
		Image* box1;
		Image* box2;

        void preload() {
            load->image("box", "assets/orangeTextbox.png");
        }

        void create() {
			game->setBackgroundColor(0, 0, 0);
            add(new FillRect(0, 0, mainCamera->width, mainCamera->height, {255, 255, 255, 255}));
			add(box1 = new Image(0, 0, "box"));
			box1->setInteractable();
			box1->setDraggable();

			add(box2 = new Image(10, 10, "box"));
			box2->setInteractable();
			box2->setDraggable();
		}

        void update() {
			cout << box1->interact.mouseHover.isDown << " | " << box2->interact.mouseHover.isDown << endl;
        }
};

int main(int argc, char** args) {
    #ifndef AMARA_WEB
        Game* game = new Game("Amara Game", false);
    #else
        WebGame* game = new WebGame("Amara Game");
    #endif
    game->init(480, 360);
    game->setWindowSize(960, 540);
    // game->setBackgroundColor(255, 255, 255);

    game->scenes->add("test", new TestScene());
    game->start("test");

    // game->scenes->add("enterSceneKey", new GiveSceneInstance());
    // game->start("enterStartingScene");

    return 0;
}
