#ifndef Amara_TaskManager
#define Amara_TaskManager

#include "amara.h"

namespace Amara {
    class TaskManager {
        public:
            Amara::GameProperties* properties = nullptr;
            std::vector<void*> deleteQueue;

            TaskManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
            }

            void queueDeletion(void* obj) {
                deleteQueue.push_back(obj);
            }

            void run() {
                void* obj;
                int size = deleteQueue.size();
                if (size > 0) {
                    std::cout << "TaskManager: Deleting " << size << " objects." << std::endl;
                }
                for (size_t i = 0; i < size; i++) {
                    obj = deleteQueue.at(i);
                    delete obj;
                }
                deleteQueue.clear();
            }
    };
}

#endif