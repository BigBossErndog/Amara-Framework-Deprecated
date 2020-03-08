#include <amara.h>
using namespace Amara;

class TestScript: public Script {
    public:
        Amara::Entity* gnik;

        void prepare(Amara::Entity& scene) {
            gnik = scene.get("teenGnik");
        }

        void script(Amara::Entity& scene) {
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

        Amara::Control* rightControl;

        TestScene() {

        }

        void preload() {
            // cout << "PRELOADING" << endl;
            load->spritesheet("teenGnikolas", "assets/teenGnikolas.png", 64, 64);
        }
        void create() {
            gnik = new Image(0, 0, "teenGnikolas");
            gnik->setOrigin(0.5);
            gnik->id = "teenGnik";
            Entity* thing = add(gnik);

            mainCamera->centerOn(gnik);

            cout << properties->resolution->width << endl;
            cout << properties->resolution->height << endl;

            recite(new TestScript());

            rightControl = controls->newControl("right");
            controls->addKey("right", K_RIGHT);
            controls->addKey("right", K_D);
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

            if (rightControl->isDown) {
                gnik->x += 1;
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