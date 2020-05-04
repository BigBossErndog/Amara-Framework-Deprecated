#pragma once
#ifndef AMARA_LOADMANAGER
#define AMARA_LOADMANAGER

#include "amara.h"

namespace Amara {
    struct LoadTask {
        Amara::AssetType type;
        bool replace;

        std::string key;
        std::string path;

        int frameWidth;
        int frameHeight;

        int size;
        int style;
        Amara::Color color;
    };

    class LoadManager: public Loader {
        public:
            Amara::Loader* load = nullptr;

            std::deque<Amara::LoadTask*> tasks;
            bool stillLoading = false;

            LoadManager(Amara::GameProperties* gameProperties): Amara::Loader(gameProperties) {
                load = properties->loader;
                reset();
            }

            void reset() {
                for (Amara::LoadTask* task : tasks) {
                    delete task;
                }
                tasks.clear();
            }

            bool remove(std::string key) {
                return load->remove(key);
            }

            int numTasks() {
                return tasks.size();
            }

            void run() {
                stillLoading = false;
                Amara::LoadTask* task;
                int count = 0;
                bool success;
                while (tasks.size() > 0 && count < loadSpeed) {
                    task = tasks.front();
                    success = false;
                    stillLoading  = true;

                    switch (task->type) {
                        case IMAGE:
                            success = load->image(task->key, task->path, task->replace);
                            break;
                        case SPRITESHEET:
                            success = load->spritesheet(task->key, task->path, task->frameWidth, task->frameHeight, task->replace);
                            break;
                        case STRINGFILE:
                            success = load->json(task->key, task->path, task->replace);
                            break;
                        case JSONFILE:
                            success = load->json(task->key, task->path, task->replace);
                            break;
                        case TTF:
                            success = load->ttf(task->key, task->path, task->size, task->color, task->style, task->replace);
                            break;
                        case SOUND:
                            success = load->sound(task->key, task->path, task->replace);
                            break;
                        case MUSIC:
                            success = load->music(task->key, task->path, task->replace);
                            break;
                    }

                    delete task;
                    tasks.pop_front();

                    if (success) {
                        count += 1;
                    }
                }
            }

            Amara::Asset* get(std::string key) {
                return load->get(key);
            }

            void pushTask(std::string key, Amara::LoadTask* asset) {
                asset->key = key;
                tasks.push_back(asset);
            }

            bool image(std::string key, std::string path, bool replace) {
                Amara::LoadTask* t  = new Amara::LoadTask();
                t->type = IMAGE;
                t->path = path;
                t->replace = replace;
                pushTask(key, t);
                return true;
            }
            bool image(std::string key, std::string path) {
                return image(key, path, false);
            }

            bool spritesheet(std::string key, std::string path, int frameWidth, int frameHeight, bool replace) {
                Amara::LoadTask* t  = new Amara::LoadTask();
                t->type = SPRITESHEET;
                t->path = path;
                t->replace = replace;
                t->frameWidth = frameWidth;
                t->frameHeight = frameHeight;
                t->replace = replace;
                pushTask(key, t);
                return true;
            }
            bool spritesheet(std::string key, std::string path, int frameWidth, int frameHeight) {
                return spritesheet(key, path, frameWidth, frameHeight, false);
            }
            bool spritesheet(std::string key, std::string path, int frameWH) {
                return spritesheet(key, path, frameWH, frameWH);
            }

            bool sound(std::string key, std::string path, bool replace) {
				Amara::LoadTask* t = new Amara::LoadTask();
                t->type = SOUND;
                t->path = path;
                t->replace = replace;
                pushTask(key, t);
                return true;
			}

			bool sound(std::string key, std::string path) {
				return sound(key, path, false);
			}

            bool music(std::string key, std::string path, bool replace) {
				Amara::LoadTask* t = new Amara::LoadTask();
                t->type = MUSIC;
                t->path = path;
                t->replace = replace;
                pushTask(key, t);
			}

            bool music(std::string key, std::string path) {
				return music(key, path, false);
			}

            bool stringFile(std::string key, std::string path, bool replace) {
                Amara::LoadTask* t  = new Amara::LoadTask();
                t->type = STRINGFILE;
                t->path = path;
                t->replace = replace;
                pushTask(key, t);
                return true;
            }
            bool stringFile(std::string key, std::string path) {
                return stringFile(key, path, false);
            }

            bool json(std::string key, std::string path, bool replace) {
                Amara::LoadTask* t  = new Amara::LoadTask();
                t->type = JSONFILE;
                t->path = path;
                t->replace = replace;
                pushTask(key, t);
                return true;
            }
            bool json(std::string key, std::string path) {
                return json(key, path, false);
            }

            bool ttf(std::string key, std::string path, int size, Amara::Color color, int style, bool replace) {
                Amara::LoadTask* t = new Amara::LoadTask();
                t->type = TTF;
                t->path = path;
                t->replace = replace;
                t->size = size;
                t->color = color;
                t->style = style;
                pushTask(key, t);
                return true;
            }

            bool ttf(std::string key, std::string path, int size, Amara::Color color, int style) {
				return ttf(key, path, size, color, style, false);
			}

			bool ttf(std::string key, std::string path, int size, Amara::Color color) {
				return ttf(key, path, size, color, TTF_STYLE_NORMAL);
			}

			bool ttf(std::string key, std::string path, int size) {
				return ttf(key, path, size, FC_MakeColor(0,0,0,255));
			}
    };
}

#endif
