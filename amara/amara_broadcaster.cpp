namespace Amara {
    class Broadcaster {
    public:
        Amara::MessageQueue* messages = nullptr;
        bool pushedMessages = false;
        
        void updateMessages() {
            if (pushedMessages) {
                pushedMessages = false;
                for (auto it = messages->begin(); it != messages->end();) {
                    Amara::Message& msg = *it;
                    if (msg.parent == this) {
                        if (msg.skip) {
                            msg.skip = false;
                            pushedMessages = true;
                        }
                        else {
                            it = messages->queue.erase(it);
                            continue;
                        }
                    }
                    ++it;
                }
            }
        }
        Amara::Message& broadcastMessage(std::string key, nlohmann::json gData) {
            pushedMessages = true;
            return messages->broadcast(this, key, gData);
        }
        Amara::Message& broadcastMessage(std::string key) {
            return broadcastMessage(key, nullptr);
        }
        Amara::Message& getMessage(std::string key) {
            return messages->get(key);
        }
        virtual void receiveMessages() {}
    };
};