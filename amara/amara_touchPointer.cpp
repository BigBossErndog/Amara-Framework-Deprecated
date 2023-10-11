namespace Amara {
    class TouchPointer: public Amara::Key, public FloatVector2 {
    public:
        Amara::GameProperties* properties = nullptr;
        SDL_FingerID id;
        
        int dx = 0;
        int dy = 0;
		int recX = 0;
		int recY = 0;
        int movementX = 0;
        int movementY = 0;
        float pressure = 0;

        bool inUse = false;

		Amara::InteractionManager* interact = nullptr;

        TouchPointer(): Amara::Key() {}
        TouchPointer(Amara::GameProperties* gProperties): Amara::TouchPointer() {
            properties = gProperties;
        }

        void manage() {
            Amara::Key::manage();
        }

        void virtualizeXY(SDL_Event& e) {
            float upScale, offset;

			recX = dx;
			recY = dy;

            x = e.tfinger.x * (float)properties->resolution->width;
            y = e.tfinger.y * (float)properties->resolution->height;
            dx = x;
            dy = y;

			if (!justDown) {
				movementX = dx - recX;
				movementY = dy - recY;
			}
			else {
				movementX = 0;
				movementY = 0;
			}

            float ratioRes = ((float)properties->resolution->width / (float)properties->resolution->height);
            float ratioWin = ((float)properties->window->width / (float)properties->window->height);

            if (ratioRes < ratioWin) {
                upScale = ((float)properties->window->height/(float)properties->resolution->height);
                offset = ((float)properties->window->width - ((float)properties->resolution->width * upScale))/2;
                dx = x + offset;
            }
            else if (ratioRes > ratioWin) {
                upScale = ((float)properties->window->width/(float)properties->resolution->width);
                offset = ((float)properties->window->height - ((float)properties->resolution->height * upScale))/2;
                dy = y + offset;
            }
        }
    };
}