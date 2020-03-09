#include <amara.h>
using namespace Amara;

class TestScript: public Script {
    public:
        Amara::Entity* gnik;

        void prepare(Amara::Entity* scene) {
            gnik = scene->get("teenGnik");
        }

        void script(Amara::Entity* scene) {
            // gnik->x += 1;

            // if (gnik->x > 50) {
            //     finish();
            // }
            finish();
        }
};

class TestScene : public Scene {
    public:
        Image* gnik;
        int c = 0;

        TestScene() {

        }

        void preload() {
            // cout << "PRELOADING" << endl;
            load->spritesheet("teenGnikolas", "assets/teenGnikolas.png", 64, 64);
        }
        void create() {
            gnik = new Image(40, 40, "teenGnikolas");
            gnik->setOrigin(0.5);
            gnik->id = "teenGnik";
            Entity* thing = add(gnik);

            cout << properties->resolution->width << endl;
            cout << properties->resolution->height << endl;

            recite(new TestScript());

            controls->setKey("up", K_UP);
            controls->setKey("down", K_DOWN);
            controls->setKey("left", K_LEFT);
            controls->setKey("right", K_RIGHT);

            mainCamera->startFollow(gnik, 0.5, 0.5);
        }
        void update() {
            // scene->stop();
            c += 1;
            if (c >= 20) {
                // gnik->frame += 1;
                c = 0;
                // mainCamera->zoomX += 0.05;
                // mainCamera->zoomY += 0.05;
            }

            if (controls->isDown("left")) {
                gnik->x -= 1;
            }
            else if (controls->isDown("right")) {
                gnik->x += 1;
            }

            if (controls->isDown("up")) {
                gnik->y -= 1;
            }
            else if (controls->isDown("down")) {
                gnik->y += 1;
            }

            if (input->mouse->left->justDown) {
                mainCamera->changeZoom(0.2);
            }
            else if (input->mouse->right->justDown) {
                mainCamera->changeZoom(-0.2);
            }
        }
};

int main(int argc, char** args) {
    Game* game = new Game("Amara Test Build");
    game->init(480, 360);
    game->setResolution(240, 180);
    game->resizeWindow(960, 720);

    game->scenes->add("test", new TestScene());
    game->start("test");

    return 0;
}