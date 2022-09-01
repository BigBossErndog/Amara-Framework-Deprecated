#pragma once


namespace Amara {
    template <typename ...Args>
    std::string stringWithFormat(const std::string& format, Args && ...args)
    {
        auto size = std::snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...);
        std::string output(size + 1, '\0');
        std::sprintf(&output[0], format.c_str(), std::forward<Args>(args)...);
        return output;
    }

    class StringParser {
    public:
        std::string content;
        int index = 0;
        int recBookmark = 0;

        StringParser(std::string gContent) {
            set(gContent);
        }
        StringParser() {}

        void reset() {
            index = 0;
            recBookmark = 0;
        }
        void set(std::string gContent) {
            content = gContent;
            reset();
        }

		std::string replace(char findC, char replaceC, bool swap) {
			std::string newContent;
			char c;
			for (int i = 0; i < content.size(); i++) {
				c = content.at(i);
				if (c == findC) {
					newContent += replaceC;
				}
				else {
					newContent += c;
				}
			}
			if (swap) content = newContent;
			return newContent;
		}

		std::string replace(char findC, char replaceC) {
			return replace(findC, replaceC, false);
		}

		std::string remove(char findC, bool swap) {
			std::string newContent;
			char c;
			for (int i = 0; i < content.size(); i++) {
				c = content.at(i);
				if (c != findC) {
					newContent += c;
				}
			}
			if (swap) content = newContent;
			return newContent;
		}
		std::string remove(char findC) {
			return remove(findC, false);
		}

        char next() {
            if (!finished()) {
                index += 1;
                return content.at(index - 1);
            }
            return '\0';
        }

		char current() {
			if (!finished()) {
				return content.at(index);
			}
			return '\0';
		}

        bool find(char f) {
            char c;
            bookmark();
            while (!finished()) {
                c = content.at(index);
                if (c == f) {
                    rewind();
                    return true;
                }
                index += 1;
            }
            rewind();
            return false;
        }

        std::string parseUntil(char c) {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);
                index += 1;

                if (r == 13) continue;
                if (r == c) {
                    return parsed;
                }
                parsed.push_back(r);
            }
            return parsed;
        }

        std::string parseUntil(std::vector<char> list) {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);
                index += 1;

                if (r == 13) continue;
                if (std::find(list.begin(), list.end(), r) != list.end()) {
                    return parsed;
                }
                parsed.push_back(r);
            }
            return parsed;
        }

        std::string parseUntilNot(char c) {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);

                if (r == 13) continue;
                if (r != c) {
                    return parsed;
                }
                parsed.push_back(r);
				index += 1;
            }
            return parsed;
        }

        std::string parseUntilNot(std::vector<char> list) {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);
				
                if (r == 13) continue;
                if (std::find(list.begin(), list.end(), r) != list.end()) {
                    return parsed;
                }
				parsed.push_back(r);
				index += 1;
            }
            return parsed;
        }

        std::string parseToEnd() {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);
                index += 1;
                if (r == 13) continue;
                parsed.push_back(r);
            }
            return parsed;
        }

        std::vector<std::string> split(char c) {
            std::vector<std::string> parsed;
            while (!finished()) {
                parsed.push_back(parseUntil(c));
            }
            reset();
            return parsed;
        }

        std::vector<std::string> split(std::vector<char> list) {
            std::vector<std::string> parsed;
            while (!finished()) {
                parsed.push_back(parseUntil(list));
            }
            reset();
            return parsed;
        }

        void bookmark() {
            recBookmark = index;
        }
        void rewind() {
            index = recBookmark;
        }

        bool finished() {
            return index >= content.size();
        }
        
        static bool isPunctuation(char c) {
            if (c >= 0x3000 && c <= 0x303f) return true;
            if (c >= 0x2000 && c <= 0x206F) return true;
            if (c >= 0x0020 && c <= 0x0040) return true;
            if (c >= 0x02B0 && c <= 0x02FF) return true;
            if (c >= 0x0300 && c <= 0x036F) return true;
            return false;
        }

        static bool isSameLanguage(char c1, char c2) {
            if (isLatinCharacter(c1) || isLatinCharacter(c2)) return true;
            if (isLatinCharacter(c1) && isLatinCharacter(c2)) return true;
            if (isJapaneseCharacter(c1) && isJapaneseCharacter(c2)) return true;
            if (isCJKCharacter(c1) && isCJKCharacter(c2)) return true;
            return false;
        }

        static bool isLatinCharacter(char c) {
            if (c >= 0x0041 && c <= 0x007F) return true;
            if (c >= 0x00A0 && c <= 0x00FF) return true;
            if (c >= 0x0100 && c <= 0x017F) return true;
            if (c >= 0x0180 && c <= 0x024F) return true;
            return false;
        }

        static bool isJapaneseCharacter(char c) {
            if (c >= 0x3040 && c <= 0x30ff) return true;
            if (c >= 0x4e00 && c <= 0x9faf) return true;

            return false;
        }

        static bool isCJKCharacter(char c) {
            if (c >= 0x4E00 && c <= 0x9FFF) return true;
            return false;
        }
    };
}