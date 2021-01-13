#include <amaraWeb.hpp>

using namespace Amara;
using namespace std;

class TestScene: public Scene {
    public:
        TextBox* box;
        Camera* exCam;
        Image* pointBox;

        void preload() {
            load->image("box", "assets/orangeTextbox.png");
            load->ttf("pressStart", "assets/press-start.regular.ttf", 8);
        }

        void create() {
            add(box = new TextBox(400, 160, "box", "pressStart"));
            box->setText("Somebody once told me the world revolved around me.");
            box->setTextColor(0, 0, 0);
            box->setProgressive();
            box->id = "TEXTBOX";

            box->setOrigin(0.5);
            box->x = game->resolution->width/2;
            box->y = game->resolution->height/2;
            box->setTextColor(0, 0, 0);

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
            controls->addKey("confirm", KEY_SPACE);

            controls->addKey("full", KEY_ESC);

            add(exCam = new Camera(10, 10, 96, 72));
            exCam->centerOn(game->width/(float)2, game->height/(float)2);
            exCam->setZoom(1/(float)5);
            box->bringToFront();

            float testAngle = 0;
            while (testAngle <= 360) {
                float rads = testAngle*M_PI/(float)180;
                float ox = sin(rads);
                float oy = cos(rads);

                cout << testAngle << ", " << ox << ", " << oy << " | ";

                float newAngle = angleBetween({0, 0}, {ox, oy});
                cout << round(newAngle*180/M_PI) << ", " << sin(newAngle) << ", " << cos(newAngle) << endl;

                testAngle += 45;
            }

            add(pointBox = new Image(0, 0, "box"));
            pointBox->setOrigin(0.5);
            pointBox->setScale(4);
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

            if (input->touches->isActivated) {
                TouchPointer* p = input->touches->pointers[0];
                pointBox->x = p->x;
                pointBox->y = p->y;
            }
            if (input->touches->isActivated) {
                cout << input->touches->pointers[0]->dx << ", " << input->touches->pointers[0]->dy << endl; 
            }

            if (controls->justDown("confirm")) {
                startTransition(new FillTransition(scenes->key));
            }

            if (controls->justDown("full")) {
                if (!game->isFullscreen) {
                    game->setWindowSize(game->display->width, game->display->height);
                    game->startFullscreen();
                }
                else {
                    game->setWindowSize(960, 720);
                    game->exitFullscreen();
                }
            }
        }
};

int main(int argc, char** args) {
    #ifndef AMARA_WEB
        Game* game = new Game("Amara Game", false);
    #else
        WebGame* game = new WebGame("Amara Game");
    #endif
    game->init(480, 360);
    game->setWindowSize(900, 600);
    // game->setBackgroundColor(255, 255, 255);

    game->scenes->add("test", new TestScene());
    game->start("test");

    // game->scenes->add("enterSceneKey", new GiveSceneInstance());
    // game->start("enterStartingScene");

    return 0;
}
