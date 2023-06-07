#include <amara.h>

using namespace Amara;
using namespace std;

// This is all test code and may not be necessarily helpful as an example.

class TestScene: public Scene, public StateManager {
    public:
		TextBox* box = nullptr;

		nlohmann::json testText;

		TrueTypeFont* showText;
		int progress = 0;

        void preload() {
            load->image("box", "assets/orangeTextbox.png");
			load->ttf("font", "assets/PixelMplus10-Regular.ttf", 10);
			load->json("testText", "assets/testText.json");
        }

        void create() {
			controls->addKey("confirm", KEY_SPACE);

			testText = assets->getJSON("testText");

			add(box = new TextBox(0, 0, 240, 68, "box", "font"));
			box->configure({
				{ "xFromCenter", -120 },
				{ "yFromCenter", -34 }
			});
			box->copyStateManager(this);

			// add(showText = new TrueTypeFont(16, 16, "font", "WAITING"));
		}

        void update() {
			start();
			for (int i = 0; i < testText.size(); i++) {
				box->say(testText[i]);
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
    game->setWindowSize(960, 720);

    game->scenes->add("test", new TestScene());
    game->start("test");

    return 0;
}
