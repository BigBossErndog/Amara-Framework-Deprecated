#ifndef AMARA_SCENEMANAGER
#define AMARA_SCENEMANAGER

#include "amara.h"

namespace Amara {
	class Game;

	class SceneManager {
		public:
			Amara::Game* game = nullptr;
			Amara::Loader* load = nullptr;
			unordered_map<string, Amara::Scene*> sceneMap;
			vector<Amara::Scene*> sceneList;

			SceneManager(Amara::Game* givenGame, Amara::Loader* givenLoad) {
				game = givenGame;
				load = givenLoad;
			}

			Amara::Scene* add(string key, Amara::Scene* scene) {
				sceneMap[key] = scene;
				sceneList.push_back(scene);
				scene->setup(game, load, new ScenePlugin(key, game, scene, &sceneMap, &sceneList));
				return scene;
			}

			Amara::Scene* remove(string key) {
				unordered_map<string, Amara::Scene*>::iterator got = sceneMap.find(key);
				if (got != sceneMap.end()) {
					sceneMap.erase(key);
					return got->second;
				}
				return nullptr;
			}

			void run() {
				Amara::Scene* scene;
				Amara::ScenePlugin* scenePlugin;

				for (size_t i = 0; i < sceneList.size(); i++) {
					scene = sceneList.at(i);
					scenePlugin = scene->scene;

					if (!scenePlugin->isActive) continue;
					if (scenePlugin->isPaused) continue;
					if (scenePlugin->isSleeping) continue;

					scene->run();
				}
			}

			void draw() {
				Amara::Scene* scene;
				Amara::ScenePlugin* scenePlugin;

				for (size_t i = 0; i < sceneList.size(); i++) {
					scene = sceneList.at(i);
					scenePlugin = scene->scene;

					if (!scenePlugin->isActive) continue;
					if (scenePlugin->isSleeping) continue;

					scene->draw();
				}
			}

			Amara::Scene* start(string key) {
				unordered_map<string, Amara::Scene*>::iterator got = sceneMap.find(key);
				if (got != sceneMap.end()) {
					Amara::Scene* scene = sceneMap[key];
					scene->scene->start();
					return scene;
				}

				return nullptr;
			}

			void manageTasks() {
				Amara::Scene* scene;
				Amara::ScenePlugin* scenePlugin;

				for (size_t i = 0; i < sceneList.size(); i++) {
					scene = sceneList.at(i);
					scenePlugin = scene->scene;

					scenePlugin->manageTasks();
				}
			}
	};
}

#endif
