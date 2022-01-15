#include <amara.h>

using namespace Amara;
using namespace std;

// This is all test code and may not be necessarily helpful as an example.

class TestScene: public Scene {
    public:
		Image* box;

        void preload() {
            load->image("box", "assets/orangeTextbox.png");
        }

        void create() {
			game->setBackgroundColor(0, 0, 0);
            add(new FillRect(0, 0, mainCamera->width, mainCamera->height, {255, 255, 255, 255}));
			add(box = new Image(0, 0, "box"));
			box->setInteractable();
			box->setDraggable();
		}

        void update() {
			cout << input->mouse->x << ", " << input->mouse->y << " | " << input->mouse->dx << ", " << input->mouse->dy << " || " << box->interact.justDown << endl;
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
