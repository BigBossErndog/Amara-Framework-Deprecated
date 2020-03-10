#ifndef AMARA_EVENTS
#define AMARA_EVENTS

#include "amara.h"

namespace Amara {
    class Interactable;

    enum EventType {
        NONE,
        SCENELEFTCLICK,
        SCENERIGHTCLICK,
        SCENEMIDDLECLICK,
        SCENELEFTRELEASE,
        SCENERIGHTRELEASE,
        SCENEMIDDLERELEASE,

        OBJECTLEFTCLICK,
        OBJECTRIGHTCLICK,
        OBJECTMIDDLECLICK,

        OBJECTLEFTRELEASE,
        OBJECTRIGHTRELEASE,
        OBJECTMIDDLERELEASE
    };

    class Event {
        public:
            Amara::EventType type;
            bool disabled = false;
            vector<Amara::Interactable*> ignore;
    };
    
    class EventManager {
        public:
            Amara::GameProperties* properties = nullptr;

            vector<Amara::Event*> eventList;

            EventManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
            }
            
            void addEvent(Amara::EventType type) {
                Amara::Event* evt = new Amara::Event();
                evt->type = type;
                eventList.push_back(evt);
            }

            void manage() {
                Amara::Event* evt;
                for (size_t i = 0; i < eventList.size(); i++) {
                    evt = eventList.at(i);
                    delete evt;
                }
                eventList.clear();
            }
    };
}

#endif