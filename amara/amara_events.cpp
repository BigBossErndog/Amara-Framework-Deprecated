#pragma once
#ifndef AMARA_EVENTS
#define AMARA_EVENTS

#include "amara.h"

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

            std::vector<Amara::Event*> eventList;
			std::vector<Amara::Event*> delayedEvents;

            EventManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                input = properties->input;
            }
            
            void addEvent(Amara::EventType type) {
                Amara::Event* evt = new Amara::Event();
                evt->type = type;
                eventList.push_back(evt);
            }
			void addDelayedEvent(Amara::EventType type) {
				Amara::Event* evt = new Amara::Event();
                evt->type = type;
                delayedEvents.push_back(evt);
			}

            void manage() {
                for (Amara::Event* evt : eventList) {
                    delete evt;
                }
                eventList = delayedEvents;
				delayedEvents.clear();

                Amara::Mouse* mouse = input->mouse;
                if (mouse->left->justDown) {
                    addEvent(OBJECTLEFTCLICK);
                }
                if (mouse->right->justDown) {
                    addEvent(OBJECTRIGHTCLICK);
                }
                if (mouse->middle->justDown) {
                    addEvent(OBJECTMIDDLECLICK);
                }
				if (mouse->left->justUp) {
                    addEvent(OBJECTLEFTRELEASE);
                }
                if (mouse->right->justUp) {
                    addEvent(OBJECTRIGHTRELEASE);
                }
                if (mouse->middle->justUp) {
                    addEvent(OBJECTMIDDLECLICK);
                }

                std::vector<TouchPointer*>& fingers = input->touches->pointers;
                for (TouchPointer* finger: fingers) {
                    if (finger->justDown) addDelayedEvent(OBJECTTOUCHDOWN);
                    if (finger->justUp) addDelayedEvent(OBJECTTOUCHUP);
                }
            }

			void manageInteracts() {
				if (input->mouse->interact) {
					input->mouse->interact->mouseHover.press();
					input->mouse->interact = nullptr;
				}
				std::vector<TouchPointer*>& fingers = input->touches->pointers;
                for (TouchPointer* finger: fingers) {
                    if (finger->interact) {
						finger->interact->touchHover.press();
						finger->interact = nullptr;
					}
                }
			}
    };
}

#endif