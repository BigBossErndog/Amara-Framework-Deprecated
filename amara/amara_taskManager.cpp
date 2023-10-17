namespace Amara {
    class Entity;
    class SceneTransitionBase;
    
    class TaskManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::vector<Amara::Entity*> entityDeletionQueue;
            std::vector<Amara::SceneTransitionBase*> transitionDeletionQueue;
            std::vector<void*> objectDeletionQueue;

            TaskManager() {}
            TaskManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
            }

            void queueDeletion(Amara::Entity* obj) {
                entityDeletionQueue.push_back(obj);
            }

            void queueDeletion(Amara::SceneTransitionBase* transition) {
                transitionDeletionQueue.push_back(transition);
            }

            void queueDeletion(void* obj) {
                objectDeletionQueue.push_back(obj);
            }

            std::vector<Amara::Entity*>& getEntityQueue() {
                return entityDeletionQueue;
            }
            std::vector<Amara::SceneTransitionBase*>& getTransitionQueue() {
                return transitionDeletionQueue;
            }
            std::vector<void*>& getObjectQueue() {
                return objectDeletionQueue;
            }

            void run() {
                clearGarbage();
            }

            void clearGarbage() {
                deleteEntities();
                deleteObjects();
                deleteTransitions();
            }

            void deleteEntities() {
                int size = entityDeletionQueue.size();
                if (properties->testing && size > 0) {
                    SDL_Log("TaskManager: Deleting %d entities.", entityDeletionQueue.size());
                }
                for (Amara::Entity* entity: entityDeletionQueue) {
                    delete entity;
                }
                entityDeletionQueue.clear();
			}

			void deleteObjects() {
                int size = objectDeletionQueue.size();
                if (properties->testing && size > 0) {
                    SDL_Log("TaskManager: Deleting %d objects.", size);
                }
                for (void* obj: objectDeletionQueue) {
                    delete obj;
                }
                objectDeletionQueue.clear();
			}

			void deleteTransitions() {
                int size = transitionDeletionQueue.size();
                if (properties->testing && size > 0) {
                    SDL_Log("TaskManager: Deleting %d transitions.", size);
                }
                for (Amara::SceneTransitionBase* transition: transitionDeletionQueue) {
                    delete transition;
                }
                transitionDeletionQueue.clear();
			}
    };
}