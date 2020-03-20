#include <amara.h>
#include "what.cpp"
using namespace Amara;

class ScriptRotation: public Script {
    public:
        void script(Amara::Entity* gnik) {
            ((Image*)gnik)->angle += 2;
        }
};

class TurnWhenClicked: public Script {
    public:
        void script(Actor* entity) {
            if (entity->isHovered) {
                entity->angle += 2;
            }
            if (entity->justClicked) {
                entity->destroy();
            }

            if (controls->isDown("up")) entity->y -= 5;
            if (controls->isDown("down")) entity->y += 5;
            if (controls->isDown("left")) entity->x -= 5;
            if (controls->isDown("right")) entity->x += 5;

            if (controls->justDown("space")) {
                entity->alpha = 1;
                cout << "HIT SPACE ";
                Amara::AudioBase* sound = audio->get("vanquisher");
                if (sound->isPlaying) {
                    sound->pause();
                    cout << "HELLO" << endl;
                }
                else {
                    cout << "WHAT" << endl;
                    sound->resume();
                }
            }
        }
};

class TestScene : public Scene {
    public:
        Layer* layer;
        Sprite* gnik;
        Canvas* canvas;
        TrueTypeFont* txt;
        bool gnikDestroyed = false;
        int c = 0;

        TestScene() {

        }

        void preload() {
            // cout << "PRELOADING" << endl;
            load->spritesheet("teenGnikolas", "assets/teenGnikolas.png", 64, 64);
            load->image("tiles", "assets/tiles.png");
            load->json("mikaelHouse_upper", "assets/mikaelHouse/mikaelHouse_upper.json");
            load->json("reeds_home", "assets/reeds_home.json");
            load->json("mikaelHouse_ground", "assets/mikaelHouse/mikaelHouse_ground.json");

            load->ttf("pressStart", "assets/press-start.regular.ttf", 8);
            
            load->music("vanquisher", "assets/vanquisher.wav");
        }
        void create() {
            audio->add("vanquisher");
            // audio->play("vanquisher");

			controls->addKey("up", K_UP);
            controls->addKey("down", K_DOWN);
            controls->addKey("left", K_LEFT);
            controls->addKey("right", K_RIGHT);

            controls->addKey("up", K_W);
            controls->addKey("down", K_S);
			controls->addKey("left", K_A);
			controls->addKey("right", K_D);

            controls->addKey("space", K_SPACE);

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
			// gnik->recite(new TestScript());

			// Sprite* gnik2;
			// add(gnik2 = new Sprite(20, 0, "teenGnikolas"));
			// gnik2->setOrigin(0.5);
			// gnik2->recite(new DelayedAnim(gnik));
            // gnik2->play("downStand");
            

            // add(new TilemapLayer("tiles", "mikaelHouse_upper"));
            Tilemap* tilemap;
            this->add(tilemap = new Tilemap("tiles", "reeds_home"));
            tilemap->createAllLayers();
            tilemap->getLayer("floor")->setCameraBounds(mainCamera);

            this->add(layer = new Container(100, 100));

            layer->add(gnik = new Sprite(0, 0, "teenGnikolas"));
            gnik->setOrigin(0.5);
            gnik->id = "teenGnik";
            gnik->play("downWalk");
            gnik->depth = 0;
            gnik->setInteractable();
            // gnik->recite(new TurnWhenClicked());
            layer->recite(new TurnWhenClicked());

            gnik->add(txt = new TrueTypeFont(0, 0, "pressStart", "Hello\nWorld"));
            txt->setColor(255, 255, 255);
            txt->setOrigin(0);
            txt->setText("Hello World\nUr Momma once told me the world revolved around me.");
            txt->alignment = FC_ALIGN_RIGHT;

            layer->setScale(2);


            // Amara::Sprite* obj;
            // for (int j = 0; j < 100; j++) {
            //     for (int i = 0; i < 100; i++) {
            //         add(obj = new Sprite(i*32,j*32,"teenGnikolas"));
            //         obj->setOrigin(0.5);
            //         obj->recite(new TestScript());
            //     }
            // }

			controls->addKey("esc", K_ESCAPE);

            mainCamera->startFollow(layer);
            // mainCamera->setZoom(2);
            // mainCamera->centerOn(100, 100);
            // mainCamera->setZoom(4); 
            // mainCamera->setZoom(1);
            // mainCamera->centerOn(100*32/2, 100*32/2);;

            Amara::Camera* cam;
            // add(cam = new Camera(10, 10, 160, 160));
            // cam->startFollow(gnik);
            // add(cam = new Camera(480-170, 360-170, 160, 160));
            // cam->setZoom(2);
            // cam->startFollow(gnik);
            // add(cam = new Camera(480-170, 10, 160, 160));
            // cam->setZoom(4);
            // cam->startFollow(gnik);
            // add(cam = new Camera(10, 360-170, 160, 160));
            // cam->setZoom(8);
            // cam->startFollow(gnik);

            // scenePlugin->start("what");
            add(canvas = new Canvas(0, 0, properties->resolution->width, properties->resolution->height));
            // canvas->setScrollFactor(0);
            canvas->beginFill(255, 255, 255);
            canvas->fillRect(0, 0, game->resolution->width, 100);
            canvas->endFill();
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
					game->startWindowedFullscreen();
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

    game->scenes->add("what", new WhatScene());
    game->scenes->add("test", new TestScene());
    game->start("test");

    return 0;
}
