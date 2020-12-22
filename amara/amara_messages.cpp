#pragma once

#include "amara.h"

namespace Amara {
    class Entity;

    typedef struct Message {
        Entity* parent = nullptr;
        std::string key;
        nlohmann::json data;

    } Message;

    class MessageQueue {
    public:
        std::vector<Message> queue;

        MessageQueue() {

        }
        
        void update() {
            for (auto it = queue.begin(); it != queue.end(); ++it) {
                Message msg = *it;
                if (msg.parent == nullptr) {
                    queue.erase(it--);
                    continue;
                }
            }
        }

        std::vector<Message>::iterator begin() {
            return queue.begin();
        }
        std::vector<Message>::iterator end() {
            return queue.end();
        }

        void clear() {
            queue.clear();
        }

        void broadcast(std::string key, nlohmann::json gData) {
            queue.push_back({ nullptr, key, gData });
        }

        void broadcast(Amara::Entity* gParent, std::string key, nlohmann::json gData) {
            queue.push_back({ gParent, key, gData });
        }
    };
}