#pragma once
#ifndef AMARA_LOADER
#define AMARA_LOADER

#include "amara.h"

namespace Amara {
    class Game;

    class Loader {
        public:
            Amara::Game* game = nullptr;
			Amara::GameProperties* properties = nullptr;
            SDL_Window* gWindow = NULL;
			SDL_Surface* gSurface = NULL;
			SDL_Renderer* gRenderer = NULL;

            std::unordered_map<std::string, Amara::Asset*> assets;
			std::string selfkey;

			bool stillLoading = false;
			int loadSpeed = 64;

			nlohmann::json emptyJson = nullptr;

            Loader(Amara::GameProperties* gameProperties) {
				properties = gameProperties;
				game = properties->game;
				gWindow = properties->gWindow;
                gSurface = properties->gSurface;
                gRenderer = properties->gRenderer;
                assets.clear();
            }

			virtual void setLoadSpeed(int speed) {
				loadSpeed = speed;
			}

            virtual Amara::Asset* get(std::string key) {
                std::unordered_map<std::string, Amara::Asset*>::iterator got = assets.find(key);
                if (got != assets.end()) {
                    return got->second;
                }
                return nullptr;
            }

			virtual bool has(std::string key) {
				if (assets.find(key) != assets.end()) {
					return true;
				}
				return false;
			}

			nlohmann::json& getJSON(std::string key) {
				Amara::JsonFile* jf = (Amara::JsonFile*)(get(key));
				if (jf != nullptr) {
					return jf->getJSON();
				}
				return emptyJson;
			}

			virtual bool remove(std::string key) {
				Amara::Asset* asset = get(key);
				if (asset != nullptr) {
					assets.erase(key);
					delete asset;
					return true;
				}
				return false;
			}

			virtual bool add(std::string key, SDL_Texture* tx, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				if (tx == nullptr) {
					std::cout << "No texture was given for key: \"" << key << "\"" <<  std::endl;
					return false;
				}
				std::cout << "Added Asset: " << key << std::endl;
				Amara::Asset* newAsset = new Amara::ImageTexture(key, IMAGE, tx);
				assets[key] = newAsset;
				return true;
			}
			virtual bool add(std::string key, SDL_Texture* tx) {
				return add(key, tx, true);
			}

			virtual bool add(std::string key, SDL_Texture* tx, int frwidth, int frheight, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				if (tx == nullptr) {
					std::cout << "No texture was given for key: \"" << key << "\"" <<  std::endl;
					return false;
				}
				std::cout << "Loaded: " << key << std::endl;
				Amara::Spritesheet* newAsset = new Amara::Spritesheet(key, SPRITESHEET, tx, frwidth, frheight);
				assets[key] = newAsset;
				return true;
			}
			virtual bool add(std::string key, SDL_Texture* tx, int frwidth, int frheight) {
				return add(key, tx, frwidth, frheight, true);
			}

			virtual bool add(std::string key, Amara::Asset* newAsset, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				std::cout << "Asset added: " << key << std::endl;
				assets[key] = newAsset;
				newAsset->key = key;
			}
			virtual bool add(std::string key, Amara::Asset* newAsset) {
				return add(key, newAsset, true);
			}

			void loadSurfacesFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					surface(key, path, replace);
				}
			}
			void loadImagesFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					image(key, path, replace);
				}
			}
			void loadSpritesheetsFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				int frameWidth, frameHeight;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					frameWidth = asset["frameWidth"];
					frameHeight = asset["frameHeight"];
					spritesheet(key, path, frameWidth, frameHeight, replace);
				}
			}
			void loadTTFsFromJSON(nlohmann::json& config) {
				// std::string key, std::string path, int size, Amara::Color color, int style, bool replace
				std::string key;
				std::string path;
				int size, style, colorSize;
				std::string strStyle;
				Amara::Color color;
				nlohmann::json jsonColor;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					size = asset["size"];

					style = TTF_STYLE_NORMAL;
					if (asset.find("style") != asset.end()) {
						nlohmann::json& jsonStyles = asset["style"];
						if (jsonStyles.is_array()) {
							for (nlohmann::json& jsonStyle: jsonStyles) {
								strStyle = jsonStyle;
								if (strStyle.compare("bold") == 0) style |= TTF_STYLE_BOLD;
								if (strStyle.compare("italic") == 0) style |= TTF_STYLE_ITALIC;
								if (strStyle.compare("underline") == 0) style |= TTF_STYLE_UNDERLINE;
								if (strStyle.compare("strikethrough") == 0) style |= TTF_STYLE_STRIKETHROUGH;
								if (strStyle.compare("outline") == 0) style |= TTF_STYLE_OUTLINE;
							}
						}
						else {
							strStyle = asset["style"];
							if (strStyle.compare("bold") == 0) style |= TTF_STYLE_BOLD;
							if (strStyle.compare("italic") == 0) style |= TTF_STYLE_ITALIC;
							if (strStyle.compare("underline") == 0) style |= TTF_STYLE_UNDERLINE;
							if (strStyle.compare("strikethrough") == 0) style |= TTF_STYLE_STRIKETHROUGH;
							if (strStyle.compare("outline") == 0) style |= TTF_STYLE_OUTLINE;
						}
					}

					color = FC_MakeColor(0,0,0,255);
					if (asset.find("color") != asset.end()) {
						jsonColor = asset["color"];
						colorSize = jsonColor.size()
;						if (colorSize >= 1) color.r = jsonColor[0];
						if (colorSize >= 2) color.g = jsonColor[1];
						if (colorSize >= 3) color.b = jsonColor[2];
						if (colorSize >= 4) color.a = jsonColor[3];
					}

					ttf(key, path, size, color, style, replace);
				}
			}
			void loadSoundsFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				int frameWidth, frameHeight;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					sound(key, path, replace);
				}
			}
			void loadMusicFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				int frameWidth, frameHeight;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					music(key, path, replace);
				}
			}
			void loadStringFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				int frameWidth, frameHeight;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					string(key, path, replace);
				}
			}
			void loadJSONFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				int frameWidth, frameHeight;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					json(key, path, replace);
				}
			}

			void loadLineByLineFromJSON(nlohmann::json& config) {
				std::string key;
				std::string path;
				int frameWidth, frameHeight;
				bool replace = false;
				for (nlohmann::json& asset: config) {
					key = asset["key"];
					path = asset["path"];
					replace = false;
					if (asset.find("replace") != asset.end()) {
						replace = asset["replace"];
					}
					lineByLine(key, path, replace);
				}
			}

			virtual bool fromJSON(std::string path) {
				bool success = true;

				std::ifstream in(path, std::ios::in | std::ios::binary);
				if (in)
				{
					std::string contents;
					in.seekg(0, std::ios::end);
					contents.resize(in.tellg());
					in.seekg(0, std::ios::beg);
					in.read(&contents[0], contents.size());
					in.close();

					std::cout << "Loaded JSON asset config file: " << path << std::endl;

					nlohmann::json config = nlohmann::json::parse(contents);
					if (config.find("surface") != config.end()) {
						loadSurfacesFromJSON(config["surface"]);
					}
					if (config.find("image") != config.end()) {
						loadImagesFromJSON(config["image"]);
					}
					if (config.find("spritesheet") != config.end()) {
						loadSpritesheetsFromJSON(config["spritesheet"]);
					}
					if (config.find("ttf") != config.end()) {
						loadTTFsFromJSON(config["ttf"]);
					}
					if (config.find("sound") != config.end()) {
						loadSoundsFromJSON(config["sound"]);
					}
					if (config.find("music") != config.end()) {
						loadMusicFromJSON(config["music"]);
					}
					if (config.find("string") != config.end()) {
						loadStringFromJSON(config["string"]);
					}
					if (config.find("json") != config.end()) {
						loadJSONFromJSON(config["json"]);
					}
					if (config.find("lineByLine") != config.end()) {
						loadLineByLineFromJSON(config["lineByLine"]);
					}
				}
				else {
					std::cout << "Loader: Failed to read file \"" << path << "\"" << std::endl;
					success = false;
				}
				return success;
			}

			virtual void reset() {}
			virtual void run() {}
			virtual int numTasks() {}

            /*
			 * Slow image.
			 */
			virtual bool surface(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;
				// Final optimized image.
				SDL_Surface* optimizedSurface = NULL;

				//Load image at specified path.
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					std::cout << "Unable to load image %s! Error: %s\n" << path << IMG_GetError() << std::endl;
					success = false;
				}
				else {
					// Convert surface to screen format.
					optimizedSurface = SDL_ConvertSurface(loadedSurface, gSurface->format, 0);

					if (optimizedSurface == NULL) {
						std::cout << "Unable to optimize image %s. SDL Error: %s\n" << path << SDL_GetError() << std::endl;
						success = false;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::Asset(key, SURFACE, optimizedSurface);
					assets[key] = newAsset;
				}

				return success;
			}

			virtual bool surface(std::string key, std::string path) {
				return surface(key, path, false);
			}

            /*
			 * Fast texture image.
			 */
			virtual bool image(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				SDL_Texture* newTexture = NULL;
				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());



				if (loadedSurface == NULL && !replace) {
					std::cout << "Unable to load image " << path << ". Error: " << IMG_GetError() << std::endl;
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						std::cout << "Unable to create texture from " << path << ". SDL Error: \n" << SDL_GetError() << std::endl;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}


				if (success) {
					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::ImageTexture(key, IMAGE, newTexture);
					assets[key] = newAsset;
				}

				return success;
			}

			virtual bool image(std::string key, std::string path) {
				return image(key, path, false);
			}

            /*
			 *  Spritesheet handles frame width and height.
			 */
			virtual bool spritesheet(std::string key, std::string path, int frwidth, int frheight, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				SDL_Texture* newTexture = NULL;

				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					std::cout << "Unable to load image " << path << ". Error: " << IMG_GetError() << std::endl;
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						std::cout << "Unable to create texture from " << path << ". SDL Error: " << SDL_GetError() << std::endl;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					std::cout << "Loaded: " << key << std::endl;
					Amara::Spritesheet* newAsset = new Amara::Spritesheet(key, SPRITESHEET, newTexture, frwidth, frheight);
					assets[key] = newAsset;
				}

				return success;
			}

			virtual bool spritesheet(std::string key, std::string path, int frwidth, int frheight) {
				return spritesheet(key, path, frwidth, frheight, false);
			}

            /*
			 * Loads a TrueTypeFont.
			 */
			virtual bool ttf(std::string key, std::string path, int size, Amara::Color color, int style, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				// Creating the font
				FC_Font* gFont = FC_CreateFont();
				FC_LoadFont(gFont, gRenderer, path.c_str(), size, color, style);
				if (gFont == nullptr) {
					success = false;
				}

				if (success) {
					std::cout << "Loaded: " << key << std::endl;
					Amara::TTFAsset* newAsset = new Amara::TTFAsset(key, TTF, gFont);

					newAsset->path = path;
					newAsset->size = size;
					newAsset->color = color;
					newAsset->style = style;

					assets[key] = newAsset;
				}

				return success;
			}

			virtual bool ttf(std::string key, std::string path, int size, Amara::Color color, int style) {
				return ttf(key, path, size, color, style, false);
			}

			virtual bool ttf(std::string key, std::string path, int size, Amara::Color color) {
				return ttf(key, path, size, color, TTF_STYLE_NORMAL);
			}

			virtual bool ttf(std::string key, std::string path, int size) {
				return ttf(key, path, size, FC_MakeColor(0,0,0,255));
			}

            virtual bool sound(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
				if (sound == NULL) {
					SDL_Log("Failed to load sound effect: \"%s\". SDL_mixer Error: %s\n", key.c_str(), Mix_GetError());
					success = false;
				}
				else {
					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::Sound(key, SOUND, sound);
					assets[key] = newAsset;
				}

				return success;
			}

			virtual bool sound(std::string key, std::string path) {
				return sound(key, path, false);
			}


			virtual bool music(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				Mix_Music* music = Mix_LoadMUS(path.c_str());
				if (music == NULL) {
					std::cout << "Failed to load music. SDL_mixer Error: %s\n" << Mix_GetError() << std::endl;
					success = false;
				}
				else {
					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::Music(key, MUSIC, music, properties);
					assets[key] = newAsset;
				}

				return success;
			}

            virtual bool music(std::string key, std::string path) {
				return music(key, path, false);
			}

			virtual bool string(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				std::ifstream in(path, std::ios::in | std::ios::binary);
				if (in)
				{
					std::string contents;
					in.seekg(0, std::ios::end);
					contents.resize(in.tellg());
					in.seekg(0, std::ios::beg);
					in.read(&contents[0], contents.size());
					in.close();

					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::StringFile(key, STRINGFILE, contents);
					assets[key] = newAsset;
				}
				else {
					std::cout << "Loader: Failed to read file \"" << path << "\"" << std::endl;
					success = false;
				}
				return success;
			}

			virtual bool json(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				std::ifstream in(path, std::ios::in | std::ios::binary);
				if (in)
				{
					std::string contents;
					in.seekg(0, std::ios::end);
					contents.resize(in.tellg());
					in.seekg(0, std::ios::beg);
					in.read(&contents[0], contents.size());
					in.close();

					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::JsonFile(key, JSONFILE, nlohmann::json::parse(contents));
					assets[key] = newAsset;
				}
				else {
					std::cout << "Loader: Failed to read file \"" << path << "\"" << std::endl;
					success = false;
				}
				return success;
			}

			virtual bool json(std::string key, std::string path) {
				json(key, path, false);
			}

			virtual bool lineByLine(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
				remove(key);
				bool success = true;

				std::ifstream in(path, std::ios::in | std::ios::binary);
				if (in) {
					std::vector<std::string> contents;
					std::string temp;
					while (std::getline(in, temp)) {
						contents.push_back(temp);
					}

					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::LineByLine(key, LINEBYLINE, contents);
					assets[key] = newAsset;
					in.close();
				}
				else {
					std::cout << "Loader: Failed to read file \"" << path << "\"" << std::endl;
					success = false;
				}

				return success;
			}

			virtual bool lineByLine(std::string key, std::string path) {
				return lineByLine(key, path, false);
			}

			virtual void regenerateAssets() {
				std::unordered_map<std::string, Amara::Asset*>::iterator it = assets.begin();
				while (it != assets.end()) {
					if (it->second->toRegenerate) {
						it->second->regenerate(gRenderer);
					}
					it++;
				}
			}
    };
}

#endif
