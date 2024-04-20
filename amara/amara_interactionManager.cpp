namespace Amara {
	class InteractionManager: public Amara::Key {
		public:
			Amara::Key mouseLeft;
			Amara::Key mouseRight;
			Amara::Key mouseMiddle;

			Amara::Key touch;

			Amara::Key mouseHover;
			Amara::Key touchHover;

			bool isDraggable = false;
			bool isBeingDragged = false;

			bool isHovered = false;
			bool justHovered = false;

			float pointX = 0;
			float pointY = 0;
			float movementX = 0;
			float movementY = 0;
			float moveDistance = 0;

			FloatVector2 firstPoint;

			bool moved = false;

			float interactScaleX = 1;
			float interactScaleY = 1;

			Amara::Mouse* mouse = nullptr;
			Amara::TouchPointer* finger = nullptr;

			Amara::FloatRect hitbox;

			void preManage() {
				tapped = mouseLeft.tapped || touch.tapped;
				
				mouseLeft.manage();
				mouseRight.manage();
				mouseMiddle.manage();

				isHovered = mouseHover.isDown || touchHover.isDown;
				justHovered = mouseHover.justDown || touchHover.justDown;

				mouseHover.manage();
				touchHover.manage();

				touch.manage();
				Amara::Key::manage();
			}
			void postManage() {
				if (mouseLeft.isDown || touch.isDown) {
					press();
				}
				else {
					release();
				}

				if (isDown) {
					if (finger) {
						setInteractPoint(finger->x, finger->y);
					}
					else if (mouse) {
						setInteractPoint(mouse->x, mouse->y);
					}
				}
				else {
					movementX = 0;
					movementY = 0;
					moved = false;
				}
			}

			void setInteractPoint(float gx, float gy) {
				if (justDown) {
					movementX = 0;
					movementY = 0;
					moved = false;
					moveDistance = 0;
					firstPoint = { gx, gy };
				}
				else {
					movementX = (gx - pointX) / interactScaleX;
					movementY = (gy - pointY) / interactScaleY;
					moveDistance = distanceBetween(0, 0, movementX, movementY);
				}
				pointX = gx;
				pointY = gy;

				if (movementX != 0 || movementY != 0) {
					moved = true;
				}
			}

			void executeEvent(EventType type) {
				switch (type) {
					case OBJECTLEFTCLICK:
						if (mouseHover.isDown) mouseLeft.press();
						break;
					case OBJECTRIGHTCLICK:
						if (mouseHover.isDown) mouseRight.press();
						break;
					case OBJECTMIDDLECLICK:
						if (mouseHover.isDown) mouseMiddle.press();
						break;
					case OBJECTLEFTRELEASE:
						if (mouseLeft.isDown) mouseLeft.release();
						break;
					case OBJECTRIGHTRELEASE:
						if (mouseRight.isDown) mouseRight.release();
						break;
					case OBJECTMIDDLERELEASE:
						if (mouseMiddle.isDown) mouseMiddle.release();
						break;
					case OBJECTTOUCHDOWN:
						if (touchHover.isDown) touch.press();
						break;
					case OBJECTTOUCHUP:
						if (touch.isDown) touch.release();
						break;
				}
			}
	};
}