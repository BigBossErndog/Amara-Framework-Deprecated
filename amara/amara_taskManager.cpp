namespace Amara {
    class Entity;
    class Script;
    class PhysicsBase;
    class SceneTransitionBase;
    class Asset;
    
    class TaskManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::vector<void*> garbageQueue;
            std::vector<void*> garbageBuffer;

            std::vector<SDL_Texture*> textureQueue;
            std::vector<SDL_Texture*> textureBuffer;

            std::vector<Amara::Entity*> entityQueue;
            std::vector<Amara::Entity*> entityBuffer;

            std::vector<Amara::Script*> scriptQueue;
            std::vector<Amara::Script*> scriptBuffer;

            std::vector<Amara::PhysicsBase*> physicsQueue;
            std::vector<Amara::PhysicsBase*> physicsBuffer;

            std::vector<Amara::SceneTransitionBase*> transitionQueue;
            std::vector<Amara::SceneTransitionBase*> transitionBuffer;

            std::vector<Amara::Asset*> assetQueue;
            std::vector<Amara::Asset*> assetBuffer;

            bool automaticGarbageClearing = true;
            bool pleaseClear = false;
            bool inTaskRunner = false;
            
            bool intervalClearing = 0;
            bool intervalCounter = 0;

            bool killLog = false;

            TaskManager() {}
            TaskManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;

                garbageQueue.clear();
                garbageBuffer.clear();

                entityQueue.clear();
                entityBuffer.clear();

                scriptQueue.clear();
                scriptBuffer.clear();

                textureQueue.clear();
                textureBuffer.clear();

                physicsQueue.clear();
                physicsBuffer.clear();

                transitionQueue.clear();
                transitionBuffer.clear();

                assetQueue.clear();
                assetBuffer.clear();
            }

            void queueEntity(Amara::Entity* entity);
            void queueScript(Amara::Script* script);
            void queuePhysics(Amara::PhysicsBase* body);
            void queueTransition(Amara::SceneTransitionBase* transition);
            void queueAsset(Amara::Asset* asset);

            void queueTexture(SDL_Texture* tx) {
                if (tx == nullptr) return;
                textureBuffer.push_back(tx);
            }

            void queueObject(void* obj) {
                if (obj == nullptr) return;
                garbageBuffer.push_back(obj);
            }

            std::vector<void*>& getObjectQueue() {
                return garbageQueue;
            }

            void run() {
                inTaskRunner = true;
                if (pleaseClear) clearGarbage();
                else if (automaticGarbageClearing) {
                    intervalCounter += 1;
                    if (intervalClearing == 0 || intervalCounter % intervalClearing == 0) {
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

                int totalGarbage = 0;

                int size = garbageQueue.size();
                if (properties->testing && !killLog && size > 0) {
                    SDL_Log("\tAmara TaskManager: Deleting %d objects.", size);
                    totalGarbage += size;
                }
                for (void* obj: garbageQueue) {
                    delete obj;
                }
                garbageQueue = garbageBuffer;
                garbageBuffer.clear();

                size = entityQueue.size();
                if (properties->testing && !killLog && size > 0) {
                    SDL_Log("\tAmara TaskManager: Deleting %d entities.", size);
                    totalGarbage += size;
                }
                for (Amara::Entity* entity: entityQueue) {
                    delete entity;
                }
                entityQueue = entityBuffer;
                entityBuffer.clear();

                size = scriptQueue.size();
                if (properties->testing && !killLog && size > 0) {
                    SDL_Log("\tAmara TaskManager: Deleting %d scripts.", size);
                    totalGarbage += size;
                }
                for (Amara::Script* script: scriptQueue) {
                    delete script;
                }
                scriptQueue = scriptBuffer;
                scriptBuffer.clear();

                size = textureQueue.size();
                if (properties->testing && !killLog && size > 0) {
                    SDL_Log("\tAmara TaskManager: Deleting %d textures.", size);
                    totalGarbage += size;
                }
                for (SDL_Texture* tx: textureQueue) {
                    SDL_DestroyTexture(tx);
                }
                textureQueue = textureBuffer;
                textureBuffer.clear();

                size = physicsQueue.size();
                if (properties->testing && !killLog && size > 0) {
                    SDL_Log("\tAmara TaskManager: Deleting %d physics bodies.", size);
                    totalGarbage += size;
                }
                for (Amara::PhysicsBase* body: physicsQueue) {
                    delete body;
                }
                physicsQueue = physicsBuffer;
                physicsBuffer.clear();

                size = transitionQueue.size();
                if (properties->testing && !killLog && size > 0) {
                    SDL_Log("\tAmara TaskManager: Deleting %d transitions.", size);
                    totalGarbage += size;
                }
                for (Amara::SceneTransitionBase* transition: transitionQueue) {
                    delete transition;
                }
                transitionQueue = transitionBuffer;
                transitionBuffer.clear();

                size = assetQueue.size();
                if (properties->testing && !killLog && size > 0) {
                    SDL_Log("\tAmara TaskManager: Deleting %d assets.", size);
                    totalGarbage += size;
                }
                for (Amara::Asset* asset: assetQueue) {
                    delete asset;
                }
                assetQueue = assetBuffer;
                assetBuffer.clear();

                if (properties->testing && !killLog && totalGarbage > 0) {
                    SDL_Log("Amara TaskManager: Total garbage: %d", totalGarbage);
                }
            }
    };
}