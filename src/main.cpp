#include <amara.h>

using namespace Amara;
using namespace std;

// This is all test code and may not be necessarily helpful as an example.

class TestScene: public Scene {
    public:
		TextureContainer* cont;
		Image* box1;
		Image* box2;

        void preload() {
            load->image("box", "assets/orangeTextbox.png");
        }

        void create() {
			game->setBackgroundColor(0, 0, 0);
            add(new FillRect(0, 0, mainCamera->width, mainCamera->height, {255, 255, 255, 255}));

			add(cont = new TextureContainer(20, 20, 100, 100));
			cont->add(new FillRect(0, 0, cont->width, cont->height, {255, 200, 200, 255}));

			cont->add(box1 = new Image(10, 10, "box"));
			box1->setInteractable();
			box1->setDraggable();
			box1->addPhysics(new PhysicsRectangle(0, 0, box1->width, box1->height));

			cont->add(box2 = new Image(64, 64, "box"));
			box2->setInteractable();
			box2->setDraggable();
			box2->addPhysics(new PhysicsRectangle(0, 0, box2->width, box2->height));

			box1->physics->setFriction(0.8);
			box2->physics->setFriction(0.8);

			box1->physics->makePushable(1);
			box2->physics->makePushable(1);

			box1->physics->addCollisionTarget(box2);
			box2->physics->addCollisionTarget(box1);

			cont->setScale(4);
		}

        void update() {
			// if (box1->interact.isDown) box1->physics->setFriction(0);
			// else box1->physics->setFriction(0.2);
			// if (box2->interact.isDown) box2->physics->setFriction(0);
			// else box2->physics->setFriction(0.2);
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
