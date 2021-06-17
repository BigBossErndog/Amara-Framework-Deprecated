#pragma once

#include "amara.h"

namespace Amara {
    class Entity;

    typedef struct Message {
        Entity* parent = nullptr;
        std::string key;
        nlohmann::json data;
        bool isActive = true;
        bool isNull = false;
        bool skip = false;
    } Message;

    class MessageQueue {
    public:
        std::vector<Message> queue;

        static Message nullMessage;

        MessageQueue() {

        }
        
        void update() {
            for (auto it = queue.begin(); it != queue.end(); ++it) {
                Message msg = *it;
                if (msg.parent == nullptr || !msg.isActive) {
                    if (msg.skip) msg.skip = false;
                    else {
                        queue.erase(it--);
                        continue;
                    }
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
            return (queue.size() == 0);
        }

        int size() {
            return queue.size();
        }

        Message& get(std::string gKey) {
            for (auto it = queue.begin(); it != queue.end(); ++it) {
                Message& msg = *it;
                if (msg.isActive && gKey.compare(msg.key) == 0) {
                    return msg;
                }
            }
            return nullMessage;
        }

        Message& broadcast(std::string key, nlohmann::json gData) {
            queue.push_back({ nullptr, key, gData });
            return queue.back();
        }

        Message& broadcast(Amara::Entity* gParent, std::string key, nlohmann::json gData) {
            queue.push_back({ gParent, key, gData });
            return queue.back();
        }
    };
    Message MessageQueue::nullMessage = { nullptr, "null", {}, false, true };
}