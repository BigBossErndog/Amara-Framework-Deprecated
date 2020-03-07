#include <amara.h>
using namespace Amara;

class TestScene : public Scene {
    public:
        Image* gnik;

        TestScene() {

        }

        void preload() {
            // cout << "PRELOADING" << endl;
            load->spritesheet("teenGnikolas", "assets/teenGnikolas.png", 64, 64);
        }
        void create() {
            game->setResolution(240, 180);
            game->resizeWindow(960, 720);
            
            gnik = new Image("teenGnikolas");
            Entity* thing = add(gnik);
        }
        void update() {
            // scene->stop();
            gnik->frame += 1;
        }
};

int main(int argc, char** args) {
    Game* game = new Game("Amara Test Build");
    game->init(480, 360);

    game->scenes->add("test", new TestScene());
    game->start("test");

    return 0;
}