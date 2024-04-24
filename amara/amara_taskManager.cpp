namespace Amara {
    class Entity;
    class SceneTransitionBase;
    
    class TaskManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::vector<void*> garbageQueue;
            std::vector<void*> garbageBuffer;

            std::vector<SDL_Texture*> textureQueue;
            std::vector<SDL_Texture*> textureBuffer;

            bool automaticGarbageClearing = true;
            bool pleaseClear = false;
            bool inTaskRunner = false;
            
            bool intervalClearing = 4;
            bool intervalCounter = 0;

            TaskManager() {}
            TaskManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;

                garbageQueue.clear();
                garbageBuffer.clear();

                textureQueue.clear();
                textureBuffer.clear();
            }

            void queueDeletion(void* obj) {
                if (obj == nullptr) return;
                garbageBuffer.push_back(obj);
            }

            void queueDeletion(SDL_Texture* tx) {
                if (tx == nullptr) return;
                textureBuffer.push_back(tx);
            }

            std::vector<void*>& getObjectQueue() {
                return garbageQueue;
            }

            void run() {
                inTaskRunner = true;
                if (pleaseClear) clearGarbage();
                else if (automaticGarbageClearing) {
                    intervalCounter += 1;
                    if (intervalCounter % intervalClearing == 0) {
                        clearGarbage();
                    }
                }
                inTaskRunner = false;
            }

            void clearGarbage() {
                if (!inTaskRunner) {
                    pleaseClear = true;
                    return;
                }
                pleaseClear = false;
                intervalCounter = 0;
                int size = garbageQueue.size();
                if (properties->testing && size > 0) {
                    SDL_Log("TaskManager: Deleting %d objects.", size);
                }
                for (void* obj: garbageQueue) {
                    delete obj;
                }
                garbageQueue = garbageBuffer;
                garbageBuffer.clear();

                size = textureQueue.size();
                if (properties->testing && size > 0) {
                    SDL_Log("TaskManager: Deleting %d textures.", size);
                }
                for (SDL_Texture* tx: textureQueue) {
                    SDL_DestroyTexture(tx);
                }
                textureQueue = textureBuffer;
                textureBuffer.clear();
            }
    };
}