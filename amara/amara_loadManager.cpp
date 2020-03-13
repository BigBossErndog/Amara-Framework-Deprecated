#ifndef AMARA_LOADMANAGER
#define AMARA_LOADMANAGER

#include "amara.h"

namespace Amara {
    struct LoadTask {
        Amara::AssetType type;
        bool replace;

        string key;
        string path;

        int frameWidth;
        int frameHeight;

        int size;
        int style;
        SDL_Color color;
    };

    class LoadManager {
        public:
            Amara::Game* game = nullptr;
            Amara::GameProperties* properties = nullptr;
            Amara::Loader* load = nullptr;

            deque<Amara::LoadTask*> tasks;
            bool stillLoading = false;

            int assetsPerFrame = 10;

            LoadManager(Amara::GameProperties* gameProperties) {
                properties = gameProperties;
                game = properties->game;
                load = properties->loader;
                reset();
            }

            void reset() {
                Amara::LoadTask* task;
                for (size_t i = 0; i < tasks.size(); i++) {
                    task = tasks.at(i);
                    delete task;
                }
                tasks.clear();
            }

            void run() {
                stillLoading = false;
                Amara::LoadTask* task;
                int count = 0;
                bool success;
                while (tasks.size() > 0 && count < assetsPerFrame) {
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
                    }

                    delete task;
                    tasks.pop_front();

                    if (success) {
                        count += 1;
                    }
                }
            }

            Amara::Asset* get(string key) {
                return load->get(key);
            }

            void pushTask(string key, Amara::LoadTask* asset) {
                asset->key = key;
                tasks.push_back(asset);
            }

            void image(string key, string path, bool replace) {
                Amara::LoadTask* t  = new Amara::LoadTask();
                t->type = IMAGE;
                t->path = path;
                t->replace = replace;
                pushTask(key, t);
            }
            void image(string key, string path) {
                image(key, path, false);
            }

            void spritesheet(string key, string path, int frameWidth, int frameHeight, bool replace) {
                Amara::LoadTask* t  = new Amara::LoadTask();
                t->type = SPRITESHEET;
                t->path = path;
                t->replace = replace;
                t->frameWidth = frameWidth;
                t->frameHeight = frameHeight;
                t->replace = replace;
                pushTask(key, t);
            }
            void spritesheet(string key, string path, int frameWidth, int frameHeight) {
                spritesheet(key, path, frameWidth, frameHeight, false);
            }
            void spritesheet(string key, string path, int frameWH) {
                spritesheet(key, path, frameWH, frameWH);
            }
    };
}

#endif
