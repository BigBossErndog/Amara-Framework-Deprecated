#ifndef AMARA_LOADER
#define AMARA_LOADER

#include "amara.h"

namespace Amara {
    class Game;
    
    class Loader {
        public:
            Amara::Game* game;
            SDL_Window* gWindow = nullptr;
			SDL_Surface* gSurface = nullptr;
			SDL_Renderer* gRenderer = nullptr;

            unordered_map<string, Amara::Asset*> assets;

            Loader(Amara::Game* givenGame) {
                game = givenGame;
            }

            void init(SDL_Window* givenWindow, SDL_Surface* givenSurface, SDL_Renderer* givenRenderer) {
                gWindow = givenWindow;
                gSurface = givenSurface;
                gRenderer = givenRenderer;
                assets.clear();
            }

            Amara::Asset* get(string key) {
                unordered_map<string, Amara::Asset*>::iterator got = assets.find(key);
                if (got != assets.end()) {
                    return got->second;
                }
                return nullptr;
            }

            /*
			 * Slow image.
			 */
			bool surface(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;
				// Final optimized image.
				SDL_Surface* optimizedSurface = NULL;

				//Load image at specified path.
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
					success = false;
				}
				else {
					// Convert surface to screen format.
					optimizedSurface = SDL_ConvertSurface(loadedSurface, gSurface->format, NULL);

					if (optimizedSurface == NULL) {
						printf("Unable to optimize image %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
						success = false;
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(SURFACE, optimizedSurface);
					assets[key] = newAsset;
				}

				return success;
			}

			bool surface(string key, string path) {
				return surface(key, path, false);
			}

            /*
			 * Fast texture image.
			 */
			bool texture(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				SDL_Texture* newTexture = NULL;

				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL && !replace) {
					printf("Unable to load image %s. Error: %s\n", path.c_str(), IMG_GetError());
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(TEXTURE, newTexture);
					assets[key] = newAsset;
				}

				return success;
			}

			bool texture(string key, string path) {
				return texture(key, path, false);
			}

            /*
			 *  Spritesheet handles frame width and height.
			 */
			bool spritesheet(string key, string path, int frwidth, int frheight, bool replace) {
				if (get(key) != nullptr && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				SDL_Texture* newTexture = NULL;

				// Load image
				SDL_Surface* loadedSurface = IMG_Load(path.c_str());
				if (loadedSurface == NULL) {
					printf("Unable to load image %s. Error: %s\n", path.c_str(), IMG_GetError());
					success = false;
				}
				else {
					// Create texture from surface pixels
					newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
					if (newTexture == NULL) {
						printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
					}

					//Get rid of old loaded surface
					SDL_FreeSurface(loadedSurface);
				}

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Spritesheet* newAsset = new Amara::Spritesheet(newTexture, frwidth, frheight);
					assets[key] = newAsset;
				}

				return success;
			}

			bool spritesheet(string key, string path, int frwidth, int frheight) {
				return spritesheet(key, path, frwidth, frheight, false);
			}

            /*
			 * Loads a TrueTypeFont.
			 */
			bool ttf(string key, string path, int size, SDL_Color color, int style, bool replace) {
				if (get(key) != nullptr && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				// Creating the font
				FC_Font* gFont = FC_CreateFont();
				FC_LoadFont(gFont, gRenderer, path.c_str(), size, color, style);

				if (success) {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(TTF, gFont);
					assets[key] = newAsset;
				}

				return success;
			}

			bool ttf(string key, string path, int size, SDL_Color color, int style) {
				return ttf(key, path, size, color, style, false);
			}

			bool ttf(string key, string path, int size, SDL_Color color) {
				return ttf(key, path, size, color, TTF_STYLE_NORMAL);
			}

			bool ttf(string key, string path, int size) {
				return ttf(key, path, size, FC_MakeColor(0,0,0,255));
			}

            bool sound(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
				if (sound == NULL) {
					printf("Failed to load sound effect. SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				else {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(SOUND, sound);
					assets[key] = newAsset;
				}

				return success;
			}

			bool sound(string key, string path) {
				return sound(key, path, false);
			}


			bool music(string key, string path, bool replace) {
				if (get(key) != nullptr && !replace) {
					printf("Loader Error: Key %s has already been used.\n", key);
					return false;
				}
				bool success = true;

				Mix_Music* music = Mix_LoadMUS(path.c_str());
				if (music == NULL) {
					printf("Failed to load music. SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				else {
					printf("Loaded: %s\n", key);
					Amara::Asset* newAsset = new Amara::Asset(MUSIC, music);
					assets[key] = newAsset;
				}

				return success;
			}

            bool music(string key, string path) {
				return music(key, path, false);
			}
    };
}

#endif