#pragma once
#ifndef AMARA_STATEMANAGER
#define AMARA_STATEMANAGER

#include "amara.h"

namespace Amara {
    struct StateRecord {
        std::string name;
		nlohmann::json data;
        int event = 0;
    };

    class StateManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::string currentState;

            std::vector<StateRecord> stateRecords;

            int currentEvent = 1;
            int eventLooker = 0;

            int waitCounter = 0;

            bool skipEvent = false;

			nlohmann::json data;

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
                jumpFlag.clear();
                stateRecords.clear();
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
                if (!currentState.empty()) {
                    Amara::StateRecord record = {currentState, data, currentEvent};
                    stateRecords.push_back(record);
                }

                currentState = key;
                currentEvent = 1;
				data.clear();
            }

            bool switchStateEvt(std::string key) {
                if (once()) {
                    switchState(key);
                    return true;
                }
                return false;
            }

            void returnState() {
                if (stateRecords.empty()) {
                    reset();
                }
                else {
                    Amara::StateRecord record = stateRecords.back();
                    currentState = record.name;
                    currentEvent = record.event;
					data = record.data;
                    stateRecords.pop_back();
                }
            }

            void returnStateEvt() {
                if (evt()) {
                    returnState();
                }
            }

            void restartState() {
                currentEvent = 1;
            }

            bool restartStateEvt() {
                if (evt()) {
                    restartState();
                    return true;
                }
                return false;
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

            bool waitUntil(bool condition) {
                if (evt()) {
                    if (condition) nextEvt();
                    return true;
                }
                return false;
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
