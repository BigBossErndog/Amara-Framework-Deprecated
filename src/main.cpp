#include <amara.h>
using namespace Amara;

class TestScene : public Scene {
    public:
        TestScene() {

        }
        void create() {
            cout << "HELLO TEST SCENE" << endl;
            game->resizeWindow(960, 720);
        }
        void update() {
            cout << "TEST SCENE IS WORKING" << endl;
            scene->stop();
        }
};

int main(int argc, char** args) {
    Game* game = new Game("Amara Test Build");
    game->init(480, 360);

    game->scenes->add("test", new TestScene());

    game->start("test");

    return 0;
}