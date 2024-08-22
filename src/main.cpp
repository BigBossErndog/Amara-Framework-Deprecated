#include <amara.h>

using namespace Amara;
using namespace std;

// This is all test code and may not be necessarily helpful as an example.
// Shows a textbox with scrolling text. Demonstrating line breaking and UTF-16 parsing.

class TestScene: public Scene, public StateManager {
    public:
		TextBox* box = nullptr;
		Video* video = nullptr;

		nlohmann::json testText;

		TrueTypeFont* showText;
		int progress = 0;

		TextureLayer* layer = nullptr;

        void preload() {
            load->image("box", "assets/orangeTextbox.png");
			load->ttf("font", "assets/PixelMplus10-Regular.ttf", 10);
			load->json("testText", "assets/testText.json");
        }

        void create() {
			reset();
			
			controls->addKey("confirm", KEY_SPACE);

			testText = assets->getJSON("testText");

			add(layer = new TextureLayer());

			layer->add(box = new TextBox(0, 0, 240, 68, "box", "font"));
			box->configure({
				{ "xFromCenter", -120 },
				{ "yFromCenter", -34 }
			});
			box->registerStateManager(this);

			add(video = new Video(scene->width/2.0, scene->height/2.0, "assets/pitv_trailer.ogv"));
			video->setRenderDelay(0.1)->setOrigin(0.5);
			video->frameSkip = 1;
		}

        void update() {
			if (state("1")) {
				if (once()) {
					video->setAlpha(1);
					video->setMasterVolume(1);
					if (video->playVideo()) {
						cout << "Video should be successfully playing." << endl;
					}
					video->scaleToFit(scene);
				}

				wait(2);

				if (evt()) {
					if (nextEvtOn(video->isFinished() || controls->isDown("confirm"))) {
						if (!video->isFinished()) {
							video->recite(new Tween_MasterVolume(video, 0, 1));
							video->recite(new Tween_Alpha(0, 1));
						}
					}
				}

				if (evt()) {
					if (nextEvtOn(video->isFinished() || video->notActing())) {
						video->stopVideo();
					}
				}

				resetEvt();
			}
        }
};

int main(int argc, char** args) {
    #ifndef AMARA_WEB
        Game game = Game("Amara Game", false);
    #else
        WebGame game = WebGame("Amara Game");
    #endif
    game.init(480, 360);
    game.setWindowSize(960, 720);
	game.setWindowPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	
	// game.debugGameLoop = true;
	// Amara::Entity::debuggingDefault = true;

    game.scenes.add("test", new TestScene());
    game.start("test");

    return 0;
}
