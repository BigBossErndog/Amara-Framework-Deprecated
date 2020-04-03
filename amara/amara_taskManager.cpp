#ifndef Amara_TaskManager
#define Amara_TaskManager

#include "amara.h"

namespace Amara {
    class Entity;
    
    class TaskManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::vector<Amara::Entity*> deleteQueue;

            TaskManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
            }

            void queueDeletion(Amara::Entity* obj) {
                deleteQueue.push_back(obj);
            }

            std::vector<Amara::Entity*>& getDeleteQueue() {
                return deleteQueue;
            }

            void run() {}
    };
}

#endif