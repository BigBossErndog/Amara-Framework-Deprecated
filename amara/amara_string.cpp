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
            std::string pText = "";
            float textWidth = 0;
            unsigned long c, lastC = 0, checkC;
            const char* end = input.c_str();
            const char* start = end;
            int seqlen = 0;
            bool otherCheck = true;
            bool lastCheck = true;
            bool newLine = false;

            while (true) {
                c = FC_ReadNextChar(end);
                seqlen = FC_GetCharSequenceLength(end);

                if (*end == '\0') break;
                newLine = false;
                if (otherCheck = StringParser::isSentenceSpacer(c)) {
                    pText = fText;
                    for (const char* p = start; p < end+seqlen; ++p) pText += *p;
                    if (FC_GetWidth(font, pText.c_str()) > wrapWidth) {
                        if (fText.size() > 0 && fText.back() != '\n') {
                            fText += '\n';
                            newLine = true;
                        }
                    }
                    for (const char* p = start; p < end+seqlen; ++p) fText += *p;

                    end += seqlen;
                    
                    if (newLine) {
                        checkC = FC_ReadNextChar(end);
                        seqlen = FC_GetCharSequenceLength(end);
                        while (StringParser::isSentenceSpacer(checkC)) {
                            end += seqlen;
                            checkC = FC_ReadNextChar(end);
                            seqlen = FC_GetCharSequenceLength(end);
                        }
                    }
                    start = end;
                }
                else if (StringParser::isCJKCharacter(c) || (!lastCheck && !StringParser::isSameLanguage(c, lastC))) {
                    pText = fText;
                    for (const char* p = start; p < end; ++p) pText += *p;
                    if (FC_GetWidth(font, pText.c_str()) > wrapWidth) {
                        if (fText.size() > 0 && fText.back() != '\n') fText += '\n';
                        while (*start == ' ') start++;
                    }
                    for (const char* p = start; p < end; ++p) fText += *p;
                    
                    start = end;
                    end += seqlen;
                }
                else end += seqlen;

                lastCheck = otherCheck;
                lastC = c;
            }

            if (start != end) {
                pText = fText;
                for (const char* p = start; p != end; ++p) pText += *p;
                if (fText.size() > 0 && fText.back() != '\n' && FC_GetWidth(font, pText.c_str()) > wrapWidth) fText += '\n';
                for (const char* p = start; p < end+seqlen; ++p) fText += *p;
            }

            return fText;
        }

        static bool isSentenceSpacer(unsigned long c) {
            if (vector_contains<unsigned long>({
                ' ', '\t'
            }, c)) return true;
            return false;
        }
        
        static bool isPunctuation(unsigned long c) {
            if (c >= 0x3000 && c <= 0x303f) return true;
            if (c >= 0x2000 && c <= 0x206F) return true;
            if (c >= 0x0020 && c <= 0x0040) return true;
            if (c >= 0x02B0 && c <= 0x02FF) return true;
            if (c >= 0x0300 && c <= 0x036F) return true;
            return false;
        }

        static bool isSameLanguage(unsigned long c1, unsigned long c2) {
            if (isPunctuation(c1) || isPunctuation(c2)) return true;
            if (isLatinCharacter(c1) && isLatinCharacter(c2)) return true;
            if (isJapaneseCharacter(c1) && isJapaneseCharacter(c2)) return true;
            if (isCJKCharacter(c1) && isCJKCharacter(c2)) return true;
            return false;
        }

        static bool isLatinCharacter(unsigned long c) {
            if (c >= 0x0041 && c <= 0x007F) return true;
            if (c >= 0x00A0 && c <= 0x00FF) return true;
            if (c >= 0x0100 && c <= 0x017F) return true;
            if (c >= 0x0180 && c <= 0x024F) return true;
            return false;
        }

        static bool isJapaneseCharacter(unsigned long c) {
            return c >= 0x3000 && c <= 0x30ff;
        }

        static bool isChineseCharacter(unsigned long c) {
            return c >= 0x4e00 && c <= 0x9FFF;
        }

        static bool isKoreanCharacter(unsigned long c) {
            return c >= 0xac00 && c <= 0xd7a3;
        }

        static bool isCJKCharacter(unsigned long c) {
            return isJapaneseCharacter(c) || isChineseCharacter(c) || isKoreanCharacter(c);
        }

        static int stringContains(std::string s, std::string f) {
            bool isSame;
            for (int i = 0; i < s.size(); i++) {
                isSame = true;
                for (int j = 0; j < f.size(); j++) {
                    if (i+j >= s.size() || s[i + j] != f[j]) {
                        isSame = false;
                        break;
                    }
                }
                if (isSame) return i;
            }
            return -1;
        }

        static std::string substr(std::string s, int startCharacter, int numCharacters) {
            std::string n;
            int count = 0;
            const char* end = s.c_str();
            const char* start = end;
            unsigned long c = FC_ReadNextChar(end);
            int seqlen = 0;

            while (count < startCharacter) {
                c = FC_ReadNextChar(end);
                seqlen = FC_GetCharSequenceLength(end);
                if (c == '\0') break;
                end += seqlen;
                count += 1;
            }

            if (c == '\0') return n;

            count = 0;
            start = end;
            while (count < numCharacters) {
                c = FC_ReadNextChar(end);
                seqlen = FC_GetCharSequenceLength(end);
                if (c == '\0') break;
                end += seqlen;
                count += 1;
            }
            for (const char* p = start; p < end; ++p) n += *p;

            return n;
        }

        static std::string substr(std::string s, int numCharacters) {
            return substr(s, 0, numCharacters);
        }

        static std::string concat(std::vector<std::string> list) {
            std::string result = "";
            for (std::string str: list) {
                result += str;
            }
            return result;
        }

        static bool startsWith(std::string s, std::string check) {
            for (int i = 0; i < check.size(); i++) {
                if (i >= s.size()) return false;
                if (s[i] != check[i]) return false;
            }
            return true;
        }
        static bool endsWith(std::string s, std::string check) {
            for (int i = 0; i < check.size(); i--) {
                if (i >= s.size()) return false;
                if (s[s.size() - i] != check[check.size()-1]) {
                    return false;
                }
            }
            return true;
        }

        // Real number of characetrs per string with respect to Unicode 16 wide characters.
        static int realSize(std::string s) {
            int count = 0;
            const char* end = s.c_str();
            unsigned long c = 0;
            int seqlen = 0;

            while (true) {
                c = FC_ReadNextChar(end);
                seqlen = FC_GetCharSequenceLength(end);
                if (c == '\0') break;
                end += seqlen;
                count += 1;
            }

            return count;
        }
    };
}