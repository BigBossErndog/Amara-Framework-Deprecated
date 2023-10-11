namespace Amara {
    class TouchManager {
    public:
        static int numTouchPointers;

        Amara::GameProperties* properties = nullptr;
        bool isActivated = false;

        std::vector<TouchPointer> pointers;
        int numFingers = 0;

        TouchPointer* lastPointer = nullptr;
        TouchPointer* activePointer = nullptr;

        FloatVector2 touchCenter = { 0, 0 };

        TouchManager() {}
        TouchManager(Amara::GameProperties* gProperties): TouchManager() {
            properties = gProperties;
            pointers.clear();
            for (int i = 0; i < numTouchPointers; i++) {
                pointers.push_back(TouchPointer(properties));
            }
        }

        TouchPointer* getPointer(SDL_FingerID gid) {
            for (TouchPointer& pointer: pointers) {
                if (pointer.inUse && pointer.id == gid) {
                    return &pointer;
                }
            }
            return nullptr;
        }

        TouchPointer* newPointer(SDL_FingerID gid) {
            for (TouchPointer& pointer: pointers) {
                if (!pointer.inUse) {
                    numFingers += 1;
                    pointer.id = gid;
                    pointer.inUse = true;
                    return &pointer;
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
            touchCenter.x = 0;
            touchCenter.y = 0;
            for (TouchPointer& pointer: pointers) {
                pointer.manage();
                if (pointer.inUse) {
                    touchCenter.x += pointer.x;
                    touchCenter.y += pointer.y;
                    isActivated = true;
                }
            }
            if (numFingers > 0) {
                touchCenter.x = touchCenter.x / numFingers;
                touchCenter.y = touchCenter.y / numFingers;
            }
        }
    };
    int TouchManager::numTouchPointers = 10;
}