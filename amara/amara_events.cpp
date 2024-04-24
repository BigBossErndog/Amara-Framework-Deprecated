namespace Amara {
    class Event {
        public:
            Amara::EventType type;
            bool disabled = false;
    };
    
    class EventManager {
        public:
            Amara::GameProperties* properties = nullptr;
            Amara::InputManager* input = nullptr;

            std::vector<Amara::Event> eventList;
			std::vector<Amara::Event> eventBuffer;

            EventManager() {}
            EventManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                input = properties->input;
            }
            
            void addEvent(Amara::EventType type) {
                Amara::Event evt;
                evt.type = type;
                eventList.push_back(evt);
            }
			void bufferEvent(Amara::EventType type) {
				Amara::Event evt;
                evt.type = type;
                eventBuffer.push_back(evt);
			}

            void manage() {
                eventList = eventBuffer;
				eventBuffer.clear();

                Amara::Mouse& mouse = input->mouse;
                if (mouse.left.justDown) {
                    addEvent(OBJECTLEFTCLICK);
                }
                if (mouse.right.justDown) {
                    addEvent(OBJECTRIGHTCLICK);
                }
                if (mouse.middle.justDown) {
                    addEvent(OBJECTMIDDLECLICK);
                }
				if (mouse.left.justUp) {
                    addEvent(OBJECTLEFTRELEASE);
                }
                if (mouse.right.justUp) {
                    addEvent(OBJECTRIGHTRELEASE);
                }
                if (mouse.middle.justUp) {
                    addEvent(OBJECTMIDDLECLICK);
                }

                std::vector<TouchPointer>& fingers = input->touches.pointers;
                for (TouchPointer& finger: fingers) {
                    if (finger.justDown) bufferEvent(OBJECTTOUCHDOWN);
                    if (finger.justUp) bufferEvent(OBJECTTOUCHUP);
                }
            }

			void manageInteracts() {
				if (input->mouse.interact) {
					input->mouse.interact->mouseHover.press();
				}
				std::vector<TouchPointer>& fingers = input->touches.pointers;
                for (TouchPointer& finger: fingers) {
                    if (finger.interact) {
						finger.interact->touchHover.press();
					}
                }
			}
    };
}