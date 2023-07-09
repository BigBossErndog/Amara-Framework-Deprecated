namespace Amara {
    class Entity;
    class SceneTransitionBase;
    
    class TaskManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::vector<Amara::Entity*> entityDeletionQueue;
            std::vector<Amara::SceneTransitionBase*> transitionDeletionQueue;
            std::vector<void*> objectDeletionQueue;

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

            void run() {}
    };
}