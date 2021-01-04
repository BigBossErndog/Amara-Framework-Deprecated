#pragma once

#include "amara.h"

namespace Amara {
    class TouchPointer: public Amara::Key, public Amara::Pointer {
    public:
        Amara::GameProperties* properties = nullptr;
        SDL_FingerID id;

        int dx = 0;
        int dy = 0;
        int x = 0;
        int y = 0;
        int movementX = 0;
        int movementY = 0;
        float pressure = 0;

        bool inUse = false;

        TouchPointer(Amara::GameProperties* gProperties): Amara::Key() {
            properties = gProperties;
        }

        void manage() {
            Amara::Pointer::manage();
        }

        void virtualizeXY(SDL_Event& e) {
            x = e.tfinger.x * (float)properties->resolution->width;
            y = e.tfinger.y * (float)properties->resolution->height;
        }
    };

    class TouchManager {
    public:
        Amara::GameProperties* properties = nullptr;
        bool isActivated = true;

        std::vector<TouchPointer*> pointers;
        int numFingers = 0;

        TouchPointer* lastPointer = nullptr;
        TouchPointer* activePointer = nullptr;

        TouchManager(Amara::GameProperties* gProperties) {
            properties = gProperties;
            pointers.clear();
            for (int i = 0; i < 10; i++) {
                pointers.push_back(new TouchPointer(properties));
            }
        }

        TouchPointer* getPointer(SDL_FingerID gid) {
            for (TouchPointer* pointer: pointers) {
                if (pointer->inUse && pointer->id == gid) {
                    return pointer;
                }
            }
            return nullptr;
        }

        TouchPointer* newPointer(SDL_FingerID gid) {
            for (TouchPointer* pointer: pointers) {
                if (!pointer->inUse) {
                    numFingers += 1;
                    pointer->id = gid;
                    pointer->inUse = true;
                    return pointer;
                }
            }
            return nullptr;
        }
        void removePointer(SDL_FingerID gid) {
            TouchPointer* pointer = getPointer(gid);
            if (pointer && pointer->inUse) {
                pointer->inUse = false;
                numFingers = false;
            }
        }

        void manage() {
            isActivated = false;
            for (TouchPointer* pointer: pointers) {
                pointer->manage();
            }
        }
    };
}