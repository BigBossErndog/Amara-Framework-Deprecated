#include <amara.h>

using namespace Amara;
using namespace std;

// This is all test code and may not be necessarily helpful as an example.
// Shows a textbox with scrolling text. Demonstrating line breaking and UTF-16 parsing.

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
			reset();
			
			controls->addKey("confirm", KEY_SPACE);

			testText = assets->getJSON("testText");

			add(box = new TextBox(0, 0, 240, 68, "box", "font"));
			box->configure({
				{ "xFromCenter", -120 },
				{ "yFromCenter", -34 }
			});
			box->registerStateManager(this);
		}

        void update() {
			start();
			for (int i = 0; i < testText.size(); i++) {
				box->say(testText[i]);
			}
			if (once()) {
				startTransition(new FillTransition("test", 0.01, 0.01));
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

    game.scenes.add("test", new TestScene());
    game.start("test");

    return 0;
}
