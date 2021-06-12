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

        StringParser(std::string gContent) {
            set(gContent);
        }
        StringParser() {}

        void reset() {
            index = 0;
        }
        void set(std::string gContent) {
            content = gContent;
            reset();
        }

        std::string parseUntil(char c) {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);
                index += 1;

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

                if (std::find(list.begin(), list.end(), r) != list.end()) {
                    return parsed;
                }
                parsed.push_back(r);
            }
            return parsed;
        }

        std::string parseToEnd() {
            std::string parsed;
            while (index < content.size()) {
                char r = content.at(index);
                index += 1;
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

        bool finished() {
            return index >= content.size();
        }
    };
}