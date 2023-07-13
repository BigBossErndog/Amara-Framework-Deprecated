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
			SDL_Log("CREATE");
			controls->addKey("confirm", KEY_SPACE);

			testText = assets->getJSON("testText");

			add(box = new TextBox(0, 0, 240, 68, "box", "font"));
			box->configure({
				{ "xFromCenter", -120 },
				{ "yFromCenter", -34 }
			});
			box->copyStateManager(this);

			// add(showText = new TrueTypeFont(16, 16, "font", "WAITING"));
			SDL_Log("END CREATE");
		}

        void update() {
			SDL_Log("UPDATE");
			start();
			for (int i = 0; i < testText.size(); i++) {
				box->say(testText[i]);
			}
			SDL_Log("END UPDATE");
        }
};

int main(int argc, char** args) {
    #ifndef AMARA_WEB
        Game* game = new Game("Amara Game", false);
    #else
        WebGame* game = new WebGame("Amara Game");
    #endif
	SDL_Log("INIT GAME");
    game->init(480, 360);
	cout << "WHERE?" << endl;
    game->setWindowSize(960, 720);
	SDL_Log("END SET WINDOW SIZE");

    game->scenes->add("test", new TestScene());
    game->start("test");

    return 0;
}
