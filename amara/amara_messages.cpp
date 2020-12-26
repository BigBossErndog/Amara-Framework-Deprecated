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

        Message nullMessage = { nullptr, "null", {} };

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

        bool empty() {
            return queue.size();
        }

        Message& get(std::string gKey) {
            for (auto it = queue.begin(); it != queue.end(); ++it) {
                Message& msg = *it;
                if (gKey.compare(msg.key) == 0) {
                    return msg;
                }
            }
            return nullMessage;
        }

        void broadcast(std::string key, nlohmann::json gData) {
            queue.push_back({ nullptr, key, gData });
        }

        void broadcast(Amara::Entity* gParent, std::string key, nlohmann::json gData) {
            queue.push_back({ gParent, key, gData });
        }
    };
}