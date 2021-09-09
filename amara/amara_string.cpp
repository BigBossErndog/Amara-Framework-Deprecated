#pragma once
#include "amara.h"

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
                index += 1;

                if (r == 13) continue;
                if (r != c) {
                    return parsed;
                }
                parsed.push_back(r);
            }
            return parsed;
        }

        std::string parseUntilNot(std::vector<char> list) {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);
                index += 1;

                if (r == 13) continue;
                if (std::find(list.begin(), list.end(), r) != list.end()) {
                    parsed.push_back(r);
                    continue;
                }
                return parsed;
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
    };
}