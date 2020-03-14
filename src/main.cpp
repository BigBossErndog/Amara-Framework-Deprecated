#include <amara.h>
using namespace Amara;

class TestScript: public Script {
    public:
        void script(Amara::Entity* gnik) {
            ((Image*)gnik)->angle += 2;
        }
};

class DelayedAnim: public Script {
	public:
		Sprite* gnik2;
		Sprite* mainGnik;
		int counter = 0;

		DelayedAnim(Sprite* givenGnik): Script() {
			mainGnik = givenGnik;
		}

		void prepare(Entity* gnik) {
			gnik2 = (Sprite*)gnik;
		}

		void script(Entity* gnik) {
			if (counter < 20) {
				counter += 1;
			}
			else if (counter == 20) {
				gnik2->play("downWalk");
				counter += 1;
                gnik2->anims->syncWith(mainGnik);
                gnik2->setInteractable();
			}
            else if (counter == 21) {
                if (gnik2->clicked) {
                    gnik2->destroy();
                    counter += 1;
                }
                else {
                    string isHover = (gnik2->hovered) ? "HOVERED" : "NOTHOVERED";
                    if (input->mouse->left->justDown) {
                        cout << "HELLO CLICK: " << isHover << endl;
                    }
                }
            }
		}
};

class TestScene : public Scene {
    public:
        Sprite* gnik;
        bool gnikDestroyed = false;
        int c = 0;

        TestScene() {

        }

        void preload() {
            // cout << "PRELOADING" << endl;
            load->spritesheet("teenGnikolas", "assets/teenGnikolas.png", 64, 64);
            load->image("tiles", "assets/tiles.png");
            load->json("mikaelHouse_upper", "assets/mikaelHouse/mikaelHouse_upper.json");
        }
        void create() {
			// controls->setKey("up", K_UP);
            // controls->setKey("down", K_DOWN);
            // controls->setKey("left", K_LEFT);
            // controls->setKey("right", K_RIGHT);
			// controls->setKey("d", K_D);
			// controls->setKey("a", K_A);
            controls->setKey("space", K_SPACE);

            Spritesheet* teenGnik = (Spritesheet*)assets->get("teenGnikolas");

            teenGnik->addAnim("downWalk", {2,3,2,4}, 6, true);
            teenGnik->addAnim("upWalk", {12,13,12,14}, 6, true);
            teenGnik->addAnim("leftWalk", {22,23,22,24}, 6, true);
            teenGnik->addAnim("rightWalk", {32,33,32,34}, 6, true);

            teenGnik->addAnim("downStand", {0,1}, 2, true);
            teenGnik->addAnim("upStand", {10,11}, 2, true);
            teenGnik->addAnim("leftStand", {20,21}, 2, true);
            teenGnik->addAnim("rightStand", {30,31}, 2, true);

            teenGnik->addAnim("downStance", 5);

            add(gnik = new Sprite(0, 0, "teenGnikolas"));
            gnik->setOrigin(0.5);
            gnik->id = "teenGnik";
            gnik->play("downWalk");
            gnik->depth = 0;
			// gnik->recite(new TestScript());

			Sprite* gnik2;
			add(gnik2 = new Sprite(20, 0, "teenGnikolas"));
			gnik2->setOrigin(0.5);
			gnik2->recite(new DelayedAnim(gnik));
            gnik2->play("downStand");
            

            // add(new TilemapLayer("tiles", "mikaelHouse_upper"));
            Tilemap* tilemap;
            add(tilemap = new Tilemap("tiles", "mikaelHouse_upper"));
            tilemap->createAllLayers();

            // Amara::Sprite* obj;
            // for (int j = 0; j < 100; j++) {
            //     for (int i = 0; i < 100; i++) {
            //         add(obj = new Sprite(i*32,j*32,"teenGnikolas"));
            //         obj->setOrigin(0.5);
            //         obj->recite(new TestScript());
            //     }
            // }

			controls->setKey("esc", K_ESCAPE);

            mainCamera->startFollow(gnik);
            // mainCamera->setZoom(1);
            // mainCamera->centerOn(100*32/2, 100*32/2);;

            // Amara::Camera* cam;
            // add(cam = new Camera(10, 10, 160, 160));
            // add(cam = new Camera(480-170, 360-170, 160, 160));
            // cam->setZoom(2);
            // add(cam = new Camera(480-170, 10, 160, 160));
            // cam->setZoom(4);
            // add(cam = new Camera(10, 360-170, 160, 160));
            // cam->setZoom(8);
        }

        void update() {

            // scene->stop();

            // if (!gnikDestroyed) {
            //     if (controls->isDown("left")) {
            //         gnik->x -= 1;
            //         gnik->play("leftWalk");
            //     }
            //     else if (controls->isDown("right")) {
            //         gnik->x += 1;
            //         gnik->play("rightWalk");
            //     }
			//
            //     if (controls->isDown("up")) {
            //         gnik->y -= 1;
            //         gnik->play("upWalk");
            //     }
            //     else if (controls->isDown("down")) {
            //         gnik->y += 1;
            //         gnik->play("downWalk");
            //     }
			//
            //     if (controls->justDown("space")) {
            //         // gnik->destroy();
            //         // gnikDestroyed = true;
            //         gnik->play("downStance");
            //     }
            // }

            // if (input->mouse->left->justDown) {
            //     mainCamera->changeZoom(0.2);
            // }
            // else if (input->mouse->right->justDown) {
            //     mainCamera->changeZoom(-0.2);
            // }

			if (controls->justDown("esc")) {
				if (game->isFullscreen) {
					game->exitFullscreen();
				}
				else {
					game->startFullscreen();
				}
			}

        }
};

int main(int argc, char** args) {
    Game* game = new Game("Amara Test Build");
    game->init(480, 360);
    game->setResolution(480, 360);
    game->resizeWindow(960, 720);
    game->setFPS(60);

    game->scenes->add("test", new TestScene());
    game->start("test");

    return 0;
}
