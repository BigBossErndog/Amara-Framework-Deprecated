namespace Amara {
    class Entity;

    typedef struct Message {
        void* parent = nullptr;
        std::string key;
        nlohmann::json data;
        bool isActive = true;
        bool isNull = false;
        bool skip = false;
    } Message;

    class MessageQueue {
    public:
        std::list<Message> queue;

        static Message nullMessage;

        MessageQueue() {}
        
        void update() {
            for (auto it = queue.begin(); it != queue.end();) {
                Message msg = *it;
                if (msg.parent == nullptr || !msg.isActive) {
                    if (msg.skip) msg.skip = false;
                    else {
                        it = queue.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }

        std::list<Message>::iterator begin() {
            return queue.begin();
        }
        std::list<Message>::iterator end() {
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

        Message& broadcast(void* gParent, std::string key, nlohmann::json gData) {
            queue.push_back({ gParent, key, gData });
            return queue.back();
        }
    };
    Message MessageQueue::nullMessage = { nullptr, "null", {}, false, true, true };
}