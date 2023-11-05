namespace Amara {
    class Interactable {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::EventManager* events = nullptr;
            Amara::InputManager* input = nullptr;

			Amara::InteractionManager interact;

			bool isInteractable = false;
			bool isDraggable = false;
			bool isBeingDragged = false;
            
            virtual void init(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                events = properties->events;
                input = properties->input;
            }

            virtual void setInteractable(bool g) {
                isInteractable = g;
            }
            virtual void setInteractable() {
                setInteractable(true);
            }

            void setDraggable(bool g) {
                isDraggable = g;
            }
            void setDraggable() {
                setDraggable(true);
            }

			void checkHover(int vx, int vy, int vw, int vh, float gx, float gy, float gw, float gh) {
				if (!isInteractable) return;
				if (gx > vw || gy > vh) return;
				if (gx + gw < 0 || gy + gw < 0) return;

				if (gx < 0) {
					gw += gx;
					gx = 0;
				}
				if (gy < 0) {
					gh += gy;
					gy = 0;
				}
				if (gx + gw > vw) {
					gw -= (gx + gw) - vw;
				}
				if (gy + gh > vh) {
					gh -= (gy + gh) - vh;
				}

				checkHover(vx + gx, vy + gy, gw, gh);
			}

			void checkHover(float gx, float gy, float gw, float gh) {
				if (!isInteractable) return;
				Amara::FloatRect box = { 
					properties->interactOffsetX + gx*properties->interactScaleX, 
					properties->interactOffsetY + gy*properties->interactScaleY,
					gw*properties->interactScaleX,
					gh*properties->interactScaleY
				};

				Amara::Mouse* mouse = &input->mouse;
				Amara::TouchManager* touches = &input->touches;
				std::vector<TouchPointer>& fingers = touches->pointers;

				bool touchHovered = false;
				bool mouseHovered = false;

				interact.mouse = mouse;
				interact.finger = nullptr;

				interact.interactScaleX = properties->interactScaleX;
				interact.interactScaleY = properties->interactScaleY;
				
				if (overlapping(mouse->dx, mouse->dy, &box)) {
					mouseHovered = true;
				}
				for (TouchPointer& finger: fingers) {
					if ((finger.isDown || finger.tapped || finger.justUp) && overlapping(finger.dx, finger.dy, &box)) {
						touchHovered = true;
						interact.finger = &finger;
					}
				}

				if (mouseHovered) {
					if (mouse->interact && mouse->interact != &interact && mouse->interact->mouseHover.isDown) {
						mouse->interact->mouseHover.release();
					}
					mouse->interact = &interact;
				}
				else {
					interact.mouseHover.release();
					interact.mouseLeft.release();
					interact.mouseRight.release();
					interact.mouseMiddle.release();
					
					interact.mouseLeft.tapped = false;
					interact.mouseRight.tapped = false;
					interact.mouseMiddle.tapped = false;
				}
				if (touchHovered) {
					if (interact.finger->interact && interact.finger->interact != &interact && interact.finger->interact->touchHover.isDown) {
						interact.finger->interact->touchHover.release();
					}
					interact.finger->interact = &interact;
				}
				else {
					interact.touchHover.release();
					interact.touch.release();

					interact.touch.tapped = false;
				}
			}

            virtual void run() {
                if (isInteractable) {
					interact.preManage();
                    for (Amara::Event* event : events->eventList) {
                        if (event->disabled) continue;
                        switch (event->type) {
                            case OBJECTLEFTCLICK:
                                if (interact.mouseHover.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTRIGHTCLICK:
								if (interact.mouseHover.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTMIDDLECLICK:
								if (interact.mouseHover.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTLEFTRELEASE:
                                if (interact.mouseLeft.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
							case OBJECTRIGHTRELEASE:
                                if (interact.mouseRight.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
							case OBJECTMIDDLERELEASE:
                                if (interact.mouseMiddle.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTTOUCHDOWN:
								if (interact.touchHover.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
                            case OBJECTTOUCHUP:
								if (interact.touch.isDown) {
									interact.executeEvent(event->type);
								}
                                break;
                        }
                    }
					interact.postManage();
                }
            }
    };
}