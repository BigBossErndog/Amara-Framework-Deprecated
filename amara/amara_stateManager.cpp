#ifndef AMARA_STATEMANAGER
#define AMARA_STATEMANAGER

#include "amara.h"

namespace Amara {
    class StateManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::string currentState;

            int currentEvent = 1;
            int eventLooker = 0;

            int waitCounter = 0;

            bool skipEvent = false;
            
            std::string jumpFlag;

            StateManager() {
                reset();
            }

            StateManager(Amara::GameProperties* gProperties) {
                properties = gProperties;
                StateManager();
            }

            void reset() {
                currentState.clear();
                currentEvent = 1;
                eventLooker = 0;
                jumpFlag.clear();
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

            bool inState(std::string key) {
                if (currentState.compare(key) == 0) {
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

            bool once() {
                if (evt()) {
                    nextEvt();
                    return true;
                }
                return false;
            }

            void nextEvt() {
                currentEvent += 1;
                skipEvent = true;
            }

            bool wait(float time) {
                bool ret = false;

                if (once()) {
                    waitCounter = 0;
                    ret = true;
                }

                if (evt()) {
                    float t;
                    if (properties != nullptr) {
                        t = properties->lps * time;
                    }
                    else {
                        t = 60 * time;
                    }

                    waitCounter += 1;
                    if (waitCounter > t) {
                        nextEvt();
                    }

                    ret = true;
                }

                return ret;
            }

            bool repeat(int num) {
                bool ret = false;
                for (int i = 0; i < num; i++) {
                    if (once()) {
                        ret = true;
                    }
                }
                return ret;
            }

            bool bookmark(std::string flag) {
                bool toReturn = false;

                if (once()) {
                    toReturn = true;
                }
                else {
                    if (jumpFlag.compare(flag) == 0) {
                        jumpFlag.clear();
                        currentEvent = eventLooker;
                        nextEvt();
                        toReturn = true;
                    }
                }

                return toReturn;
            }

            void jump(std::string flag) {
                jumpFlag = flag;
            }

            bool jumpEvt(std::string flag) {
                if (evt()) {
                    jump(flag);
                    return true;
                }
                return false;
            }
    };
}

#endif
