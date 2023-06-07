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

        static std::string wrapString(FC_Font* font, std::string input, float wrapWidth) {
            std::string recText = input;
            std::string fText = "";
            std::string word = "";
            std::string pText = "";
            float textWidth = 0;
            char c, lastC = 0;

            for (int i = 0; i < input.length(); i++) {
                c = input.at(i);
                
                if (c == ' ') {
                    pText = fText + word;
                    if (FC_GetWidth(font, pText.c_str()) > wrapWidth) {
                        fText += '\n';
                        fText += word;
                    }
                    else {
                        fText += word;
                    }

                    pText = fText + c;
                    if (FC_GetWidth(font, pText.c_str()) > wrapWidth) {
                        fText += '\n';
                    }
                    else {
                        fText += c;
                    }

                    word.clear();
                }
                else {
                    if (StringParser::isPunctuation(c)) {
                        word += c;
                    }
                    else if (!StringParser::isSameLanguage(lastC, c)) {
                        pText = fText + word;
                        if (FC_GetWidth(font, pText.c_str()) > wrapWidth) {
                            fText += '\n';
                            fText += word;
                        }
                        else {
                            fText += word;
                        }
                        word = c;
                    }
                    else if (StringParser::isCJKCharacter(c)) {
                        pText = fText + word;
                        if (FC_GetWidth(font, pText.c_str()) > wrapWidth) {
                            fText += L'\n';
                            fText += word;
                        }
                        else {
                            fText += word;
                        }
                        word = c;
                    }
                    else {
                        word += c;
                    }
                }
                lastC = c;
            }

            pText = fText + word;

            if (FC_GetWidth(font, pText.c_str()) > wrapWidth) {
                fText += '\n';
                fText += word;
            }
            else {
                fText += word;
            }

            return fText;
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
            std::regex expr("[\u3040-\u30ff]");
            std::string strC = std::string(1, c);
            return std::regex_match(strC, expr);
        }

        static bool isChineseCharacter(char c) {
            std::regex expr("[\u4e00-\u9FFF]");
            std::string strC = std::string(1, c);
            return std::regex_match(strC, expr);
        }

        static bool isKoreanCharacter(char c) {
            std::regex expr("[\uac00-\ud7a3]");
            std::string strC = std::string(1, c);
            return std::regex_match(strC, expr);
        }

        static bool isCJKCharacter(char c) {
            return isJapaneseCharacter(c) || isChineseCharacter(c) || isKoreanCharacter(c);
        }
    };
}