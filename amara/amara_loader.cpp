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
			int loadSpeed = 100;

            Loader(Amara::GameProperties* gameProperties) {
				properties = gameProperties;
				game = properties->game;
				gWindow = properties->gWindow;
                gSurface = properties->gSurface;
                gRenderer = properties->gRenderer;
                assets.clear();
            }

            virtual Amara::Asset* get(std::string key) {
                std::unordered_map<std::string, Amara::Asset*>::iterator got = assets.find(key);
                if (got != assets.end()) {
                    return got->second;
                }
                return nullptr;
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
					if (got != nullptr) {
						delete got;
					}
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
					if (got != nullptr) {
						delete got;
					}
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
					if (got != nullptr) {
						delete got;
					}
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
					if (got != nullptr) {
						delete got;
					}
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
				bool success = true;

				Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
				if (sound == NULL) {
					std::cout << "Failed to load sound effect. SDL_mixer Error: %s\n" <<  Mix_GetError() << std::endl;
					success = false;
				}
				else {
					std::cout << "Loaded: " << key << std::endl;
					Amara::Asset* newAsset = new Amara::Sound(key, SOUND, sound);
					assets[key] = newAsset;
					if (got != nullptr) {
						delete got;
					}
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
					if (got != nullptr) {
						delete got;
					}
				}

				return success;
			}

            virtual bool music(std::string key, std::string path) {
				return music(key, path, false);
			}

			virtual bool stringFile(std::string key, std::string path, bool replace) {
				Amara::Asset* got = get(key);
				if (got != nullptr && !replace) {
					std::cout << "Loader: Key \"" << key << "\" has already been used." << std::endl;
					return false;
				}
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
					if (got != nullptr) {
						delete got;
					}
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
					if (got != nullptr) {
						delete got;
					}
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
    };
}

#endif
