#include <amara.h>
using namespace Amara;

class TestScript: public Script {
    public:
        void script(Amara::Entity* gnik) {
            ((Image*)gnik)->angle += 2;
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
        }
        void create() {
            assets->addAnim("teenGnikolas", "walk", {2,3,2,4}, 6, true);
            assets->addAnim("teenGnikolas", "downStance", 5);

            add(gnik = new Sprite(0, 0, "teenGnikolas"));
            gnik->setOrigin(0.5);
            gnik->id = "teenGnik";
            gnik->play("walk");
            
            // Amara::Sprite* obj;
            // for (int j = 0; j < 100; j++) {
            //     for (int i = 0; i < 100; i++) {
            //         add(obj = new Sprite(i*32,j*32,"teenGnikolas"));
            //         obj->setOrigin(0.5);
            //         obj->play("walk");
            //     }
            // }

            controls->setKey("up", K_UP);
            controls->setKey("down", K_DOWN);
            controls->setKey("left", K_LEFT);
            controls->setKey("right", K_RIGHT);
			controls->setKey("d", K_D);
			controls->setKey("a", K_A);
            controls->setKey("space", K_SPACE);

			controls->setKey("esc", K_ESCAPE);

            mainCamera->startFollow(gnik);
            mainCamera->setZoom(4);
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
            c += 1;
            if (c >= 20) {
                // gnik->frame += 1;
                c = 0;
                // mainCamera->zoomX += 0.05;
                // mainCamera->zoomY += 0.05;
            }

            if (!gnikDestroyed) {
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

                if (controls->justDown("space")) {
                    // gnik->destroy();
                    // gnikDestroyed = true;
                    gnik->play("downStance");
                }
            }

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
    // game->setFPS(30, 60);

    game->scenes->add("test", new TestScene());
    game->start("test");

    return 0;
}
