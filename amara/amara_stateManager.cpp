#ifndef AMARA_STATEMANAGER
#define AMARA_STATEMANAGER

#include <amara.h>

namespace Amara {
    class StateManager {
        public:
            std::string currentState;

            int currentEvent = 1;
            int eventLooker = 0;

            bool skipEvent = false;


            StateManager() {
                reset();
            }

            void reset() {
                currentState.clear();
                currentEvent = 1;
                eventLooker = 0;
            }

            bool state(std::string key) {
                if (currentState.empty()) {
                    currentState = key;
                }

                if (currentState.compare(key) == 0) {
                    eventLooker = 0;
                    return true;
                }

                return false;
            }

            bool start() {
                if (currentState.empty()) {
                    eventLooker = 0;
                    return true;
                }
                return false;
            }

            void switchState(std::string key) {
                currentState = key;
                currentEvent = 1;
            }

            bool evt() {
                eventLooker += 1;
                if (currentEvent == eventLooker) {

                    if (skipEvent) {
                        skipEvent = false;
                        return false;
                    }

                    return true;
                }
                return false;
            }

            void nextEvt() {
                currentEvent += 1;
                skipEvent = true;
            }
    };
}

#endif
