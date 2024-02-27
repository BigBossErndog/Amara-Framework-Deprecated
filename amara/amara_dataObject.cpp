namespace Amara {
    class DataObject {
    public:
        nlohmann::json data = nullptr;

        virtual void configure(nlohmann::json config) {}

        void configure(std::string key, nlohmann::json val) {
            nlohmann::json config = nlohmann::json::object();
            config[key] = val;
            configure(config);
        }
        
        virtual nlohmann::json toData() {
            return data;
        }

        bool hasDataProperty(std::string gKey) {
			if (data.find(gKey) != data.end()) {
				return true;
			} 
			return false;
		}

		bool isDataProperty(std::string gKey) {
			if (hasDataProperty(gKey) && data[gKey].is_boolean() && data[gKey]) {
				return true;
			}
			return false;
		}

		bool eraseDataProperty(std::string key) {
			if (hasDataProperty(key)) {
                data.erase(key);
                return true;
            }
            return false;
		}
    };
}