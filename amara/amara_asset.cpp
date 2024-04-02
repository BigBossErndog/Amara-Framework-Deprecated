namespace Amara {
    enum AssetType {
        JSONCONFIG,
        ASSETPATH,
        SURFACE,
        IMAGE,
		CIRCLETEXTURE,
        RADIALGRADIENTTEXTURE,
		GRADIENTTEXTURE,
        SPRITESHEET,
        TTF,
        SOUND,
        MUSIC,
        JSONFILE,
        STRINGFILE,
        LINEBYLINE,
        CSVFILE
    };

    class Asset {
        public:
            AssetType type;
            std::string key;

            bool toRegenerate = false;

            Asset() {}

            Asset(std::string givenKey, AssetType givenType) {
                key = givenKey;
                type = givenType;
            }

            virtual void regenerate(SDL_Renderer*) {}
    };

    class SurfaceAsset: public Amara::Asset {
        public:
            SDL_Surface* asset = nullptr;

            SurfaceAsset(std::string givenKey, AssetType givenType, SDL_Surface* surface): Amara::Asset(givenKey, givenType) {
                asset = surface;
            }

            ~SurfaceAsset() {
                SDL_FreeSurface(asset);
            }
    };

    class ImageTexture : public Amara::Asset {
        public:
            int width = 0;
            int height = 0;

            bool temp = false;

            SDL_Texture* asset = nullptr;

            ImageTexture(std::string key, AssetType givenType, SDL_Texture* givenAsset): Amara::Asset(key, givenType) {
                setTexture(givenAsset);
            }

            void removeTexture() {
                if (asset) SDL_DestroyTexture(asset);
                asset = nullptr;
            }

            void setTexture(SDL_Texture* givenAsset) {
                if (givenAsset == nullptr) return;
                removeTexture();
                SDL_QueryTexture(givenAsset, NULL, NULL, &width, &height);
                asset = givenAsset;
            }

            virtual ~ImageTexture() {
                removeTexture();
            }
    };

    class RadialGradientTexture: public Amara::ImageTexture {
        public:
            SDL_Color innerColor;
            SDL_Color outerColor;
            float fadeStart;
            
            RadialGradientTexture(std::string key, AssetType givenType, SDL_Texture* givenAsset): Amara::ImageTexture(key, givenType, givenAsset) {}

            void configure(SDL_Color gInnerColor, SDL_Color gOuterColor, float gFadeStart) {
                innerColor = gInnerColor;
                outerColor = gOuterColor;
                fadeStart = gFadeStart;

                toRegenerate = true;
            }

            void regenerate(SDL_Renderer* gRenderer) {
                SDL_DestroyTexture(Amara::ImageTexture::asset);
                Amara::ImageTexture::asset = createRadialGradientTexture(gRenderer, width, height, innerColor, outerColor, fadeStart);
            }
    };

	class CircleTexture: public Amara::ImageTexture {
		public:
			SDL_Color color;
			float radius;

			CircleTexture(std::string key, AssetType givenType, SDL_Texture* givenAsset): Amara::ImageTexture(key, givenType, givenAsset) {}

			void configure(float gRadius, SDL_Color gColor) {
				color = gColor;
				radius = gRadius;

				toRegenerate = true;
			}

			void regenerate(SDL_Renderer* gRenderer) {
				SDL_DestroyTexture(Amara::ImageTexture::asset);
				Amara::ImageTexture::asset = createCircleTexture(gRenderer, radius, color);
			}
	};

	class GradientTexture: public Amara::ImageTexture {
		public:
			Amara::Direction dir = NoDir;
			SDL_Color colorIn;
			SDL_Color colorOut;

			GradientTexture(std::string key, AssetType givenType, SDL_Texture* givenAsset): Amara::ImageTexture(key, givenType, givenAsset) {}

			void configure(Amara::Direction gDir, SDL_Color gColorIn, SDL_Color gColorOut) {
				dir = gDir;
				colorIn = gColorIn;
				colorOut = gColorOut;

				toRegenerate = true;
			}

			void regenerate(SDL_Renderer* gRenderer) {
				SDL_DestroyTexture(Amara::ImageTexture::asset);
				Amara::ImageTexture::asset = createGradientTexture(gRenderer, width, height, dir, colorIn, colorOut);
			}
	};

    class Spritesheet: public Amara::ImageTexture {
        public:
            int frameWidth = 0;
            int frameHeight = 0;

            std::unordered_map<std::string, Amara::Animation> anims;

            Spritesheet(std::string key, AssetType givenType, SDL_Texture* newtexture, int newwidth, int newheight): Amara::ImageTexture(key, givenType, nullptr) {
                setTexture(newtexture, newwidth, newheight);
            }

            using Amara::ImageTexture::setTexture;
            void setTexture(SDL_Texture* newtexture, int newwidth, int newheight) {
                setTexture(newtexture);

                frameWidth = newwidth;
                frameHeight = newheight;
                if (frameWidth > width) {
                    frameWidth = width;
                }
                if (frameHeight > height) {
                    frameHeight = height;
                }
            }

            Amara::Animation* addAnim(std::string animKey, std::vector<int> frames, int frameRate, bool loop) {
                auto got = anims.find(animKey);
                if (got != anims.end()) {
                    Amara::Animation& anim = got->second;
                    anim.frames = frames;
                    anim.frameRate = frameRate;
                    anim.loop = loop;
                    return &anim;
                }
                anims[animKey] = Amara::Animation(key, animKey, frames, frameRate, loop);
                return &anims[animKey];
            }
            Amara::Animation* addAnim(std::string animKey, int frame) {
                return addAnim(animKey, {frame}, 1, false);
            }

            Amara::Animation* getAnim(std::string animKey) {
                auto got = anims.find(animKey);
                if (got != anims.end()) {
                    return &got->second;
                }
                return nullptr;
            }

            void removeAnim(std::string animKey) {
                auto got = anims.find(animKey);
                if (got != anims.end()) {
                    anims.erase(animKey);
                }
            }
    };

    class StringFile: public Amara::Asset {
        public:
            std::string contents;

            StringFile(std::string givenKey, AssetType givenType, std::string gContents): Amara::Asset(givenKey, STRINGFILE) {
                contents = gContents;
            }

            std::string getString() {
                return contents;
            }

            std::string toString() {
                return getString();
            }
    };

	class JsonFile: public Amara::Asset {
		public:
            nlohmann::json jsonObj;

			JsonFile(std::string givenKey, AssetType givenType, nlohmann::json gJson): Amara::Asset(givenKey, JSONFILE) {
                jsonObj = gJson;
            }

            nlohmann::json& getJSON() {
                return jsonObj;
            }
	};

    class TTFAsset: public Amara::Asset {
        public:
            FC_Font* font = nullptr;
            std::string path;
            int size;
            SDL_Color color;
            int style;

            bool recFullscreen = false;

            TTFAsset(std::string givenKey, AssetType givenType, FC_Font* gFont): Amara::Asset(givenKey, TTF) {
                font = gFont;
                toRegenerate = true;
            }

            void reloadFontCache(SDL_Renderer* gRenderer) {
                FC_ClearFont(font);
                FC_LoadFont(font, gRenderer, path.c_str(), size, color, style);
            }

            void regenerate(SDL_Renderer* gRenderer) {
                reloadFontCache(gRenderer);
            }
    };

    class LineByLine: public Amara::Asset {
    public:
        int index = 0;
        std::vector<std::string> contents;

        LineByLine(std::string givenKey, AssetType givenType, std::vector<std::string> givenContents): Amara::Asset(givenKey, LINEBYLINE){
            contents = givenContents;
        }

        void reset() {
            index = 0;
        }

        std::string getLine() {
            index += 1;
            if (index > contents.size()) {
                index -= 1;
                return "EOF";
            }
            else {
                return contents[index - 1];
            }
        }

        std::string getLine(int i) {
            return contents[i];
        }

        bool eof() {
            return index >= contents.size();
        }
    };

    class CSVLine {
    public:
        std::vector<nlohmann::json> items;
        bool invalid = false;
        
        int itemCount = 0;
        std::string originalLine;

        CSVLine() {}
        CSVLine(std::string line) {
            setLine(line);
        }
        CSVLine(bool gInvalid) {
            invalid = gInvalid;
        }

        char getChar(std::string line, int index) {
            if (index < 0 || index >= line.size()) return 0;
            return line.at(index);
        }

        bool acceptItem(std::string stringItem, bool throwItem) {
            if (stringItem.size() == 0) {
                if (!throwItem) items.push_back(stringItem);
                return false;
            }
            if (stringItem.compare("TRUE") == 0) {
                items.push_back(true);
            }
            else if (stringItem.compare("FALSE") == 0) {
                items.push_back(false);
            }
            else if (nlohmann::json::accept(stringItem)) {
                nlohmann::json obj = nlohmann::json::parse(stringItem);
                if (obj.is_string()) items.push_back(stringItem);
                else items.push_back(obj);
            }
            else {
                items.push_back(stringItem);
            }
            itemCount = items.size();
            return true;
        }

        void setLine(std::string line) {
            originalLine = line;

            char c;
            std::string stringItem = "";
            bool inString = false;
            bool escapeChar = false;

            itemCount = 0;
            items.clear();

            for (int i = 0; i < line.size(); i++) {
                c = line.at(i);

                if (c == '\r' || c == '\0') break;

                if (c == '"') {
                    if (!inString) {
                        inString = true;
                    }
                    else if (escapeChar) {
                        stringItem += c;
                        escapeChar = false;
                    }
                    else if (getChar(line, i+1) == ',') {
                        inString = false;
                    }
                    else {
                        escapeChar = true;
                    }
                }
                else if (!inString && c == ',') {
                    acceptItem(stringItem, false); 
                    stringItem = "";
                }
                else {
                    stringItem += c;
                }
            }
            acceptItem(stringItem, false);
            while (items.size() > itemCount) {
                items.pop_back();
            }
        }

        nlohmann::json get(int index) {
            if (index < 0 || index >= items.size()) return "INVALID";
            return items[index];
        }
        
        int size() {
            return items.size();
        }
    };

    class CSVFile: public Amara::Asset {
    public:
        int index = 0;
        std::vector<CSVLine> lines;

        Amara::CSVLine invalidLine = Amara::CSVLine(true);

        CSVFile(std::string givenKey, AssetType givenType, std::vector<std::string> givenContents): Amara::Asset(givenKey, LINEBYLINE){
            lines.clear();
            lines.resize(givenContents.size());
            for (int i = 0; i < givenContents.size(); i++) {
                lines[i].setLine(givenContents[i]);
            }
        }

        void reset() {
            index = 0;
        }

        Amara::CSVLine& getLine() {
            index += 1;
            if (index > lines.size()) {
                index -= 1;
                return invalidLine;
            }
            else {
                return lines[index - 1];
            }
        }

        Amara::CSVLine& getLine(int i) {
            return lines[i];
        }

        bool eof() {
            return index >= lines.size();
        }

        int size() {
            return lines.size();
        }
    };
}
