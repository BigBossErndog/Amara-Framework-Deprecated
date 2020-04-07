#include <amara.h>

using namespace Amara;
using namespace std;

class TestScene: public Scene {
    public:
        Textbox* box;
        void preload() {
            load->image("box", "assets/orangeTextbox.png");
            load->ttf("pressStart", "assets/press-start.regular.ttf", 16);
        }

        void create() {
            add(box = new Textbox(400, 160, "box", "pressStart"));

            controls->addKey("up", KEY_UP);
            controls->addKey("down", KEY_DOWN);
            controls->addKey("left", KEY_LEFT);
            controls->addKey("right", KEY_RIGHT);

            controls->addButton("up", BUTTON_DPAD_UP);
            controls->addButton("down", BUTTON_DPAD_DOWN);
            controls->addButton("left", BUTTON_DPAD_LEFT);
            controls->addButton("right", BUTTON_DPAD_RIGHT);

            controls->addButton("left", BUTTON_TRIGGERLEFT);
            controls->addButton("right", BUTTON_TRIGGERRIGHT);

            controls->addButton("up", LEFTSTICK_UP);
            controls->addButton("down", LEFTSTICK_DOWN);
            controls->addButton("left", LEFTSTICK_LEFT);
            controls->addButton("right", LEFTSTICK_RIGHT);

            controls->addButton("confirm", BUTTON_A);
        }

        void update() {
            int neww = box->width, newh = box->height;
            if (controls->isDown("up")) {
                log("UP PRESSED");
                box->height -= 1;
            }
            if (controls->isDown("down")) {
                log("DOWN PRESSED");
                box->height += 1;
            }
            if (controls->isDown("left")) {
                log("LEFT PRESSED");
                box->width -= 1;
            }
            if (controls->isDown("right")) {
                log("RIGHT PRESSED");
                box->width += 1;
            }

            if (controls->justDown("confirm")) {
                cout << input->gamepads->numConnected() << endl;
            }
        }
};

int main(int argc, char** args) {
    Game* game = new Game("Amara Game");
    game->init(480, 360);

    game->scenes->add("test", new TestScene());
    game->start("test");

    // game->scenes->add("enterSceneKey", new GiveSceneInstance());
    // game->start("enterStartingScene");

    return 0;
}
