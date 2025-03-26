namespace Amara {
    class RNG {
        public:
            std::string lastSeed;
            std::random_device rd;
            std::mt19937 e;

            RNG() {
                randomize();
            }

            RNG* randomize() {
                e.seed(time(0));
				return this;
            }

			RNG* seed(int s) {
				e.seed(s);
				return this;
			}

            RNG* seed(std::string s, uint32_t st) {
                uint32_t sn; 
                MurmurHash3_x86_32(s.c_str(), s.size(), st, &sn);
				seed(sn);
                lastSeed = s;
				return this;
            }

			RNG* seed(std::string s) {
                return seed(s, 0);
			}

            double between(double min, double max) {
                std::uniform_real_distribution<double> dist(min, max);
                double result = dist(e);
                return result;
            }

            double random() {
                return between(0, 1);
            }

            int intBetween(int min, int max) { // ends inclusive
                return floor(between(min, max+1));
            }

            std::string randomString(char min, char max, int length) {
                std::string toReturn = "";
                char c;
                for (int i = 0; i < length; i++) {
                    c = floor(min + random() * (max+1 - min));
                    toReturn += c;
                }
                return toReturn;
            }

            std::string randomString(int length) {
                return randomString('a', 'z', length);
            }

			nlohmann::json randomItem(nlohmann::json list) {
				if (list.is_array()) {
					return list[random() * list.size()];
				}
				return list;
			}

            SDL_Color randomColor(int alpha) {
                return {
                    (Uint8)intBetween(0, 255),
                    (Uint8)intBetween(0, 255),
                    (Uint8)intBetween(0, 255),
                    (Uint8)alpha
                };
            }
            SDL_Color randomColor() {
                return randomColor(255);
            }

            std::vector<nlohmann::json> randomItems(nlohmann::json list, int numItems, bool useItemOnce) {
                std::vector<nlohmann::json> newList;
                int index;
                for (int i = 0; i < numItems; i++) {
                    if (list.size() == 0) break;
                    index = floor(random() * list.size());
                    newList.push_back(list[index]);
                    if (useItemOnce) list.erase(list.begin() + index);
                }
                return newList;
            }
            std::vector<nlohmann::json> randomItems(nlohmann::json list, int numItems) {
                return randomItems(list, numItems, true);
            }

            template <typename T >int randomIndex(std::vector<T> list) {
                return floor(random()*list.size());
            }

            template <typename T> T randomItem(std::vector<T> list) {
                return list[randomIndex<T>(list)];
            }

            IntVector2 spotWithin(IntRect rect) {
                return {
                    (int)(rect.x + floor(random()*rect.width)),
                    (int)(rect.y + floor(random()*rect.height))
                };
            }
            FloatVector2 spotWithin(FloatRect rect) {
                return {
                    (float)(rect.x + random()*rect.width),
                    (float)(rect.y + random()* rect.height)
                };
            }

            nlohmann::json shuffle(nlohmann::json list) {
                if (list.is_array()) {
                    std::shuffle(std::begin(list), std::end(list), e);
                }
                return list;
            }

            bool roll(double check) {
                return random() < check;
            }
            bool flip() {
                return roll(0.5);
            }
            bool determine(std::string s, double check) {
                seed(s);
                return roll(check);
            }
            std::string determineString(std::string s, int num) {
                seed(s);
                return randomString(num);
            }
    };
}
