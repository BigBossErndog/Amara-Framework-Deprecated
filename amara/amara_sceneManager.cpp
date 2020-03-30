#ifndef AMARA_SCENEMANAGER
#define AMARA_SCENEMANAGER

#include "amara.h"

namespace Amara {
	class Game;

	class SceneManager {
		public:
			Amara::Game* game = nullptr;
			Amara::GameProperties* properties = nullptr;
			std::unordered_map<std::string, Amara::Scene*> sceneMap;
			std::vector<Amara::Scene*> sceneList;

			SceneManager(Amara::GameProperties* gameProperties) {
				properties = gameProperties;
				game = properties->game;
			}

			Amara::Scene* add(std::string key, Amara::Scene* scene) {
				sceneMap[key] = scene;
				sceneList.push_back(scene);
				scene->setup(properties, new ScenePlugin(key, properties, scene, &sceneMap, &sceneList));
				std::cout << "ADDED SCENE: " << scene->scenePlugin->key << std::endl;
				return scene;
			}

			Amara::Scene* remove(std::string key) {
				std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap.find(key);
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
					scenePlugin = scene->scenePlugin;

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
					scenePlugin = scene->scenePlugin;

					if (!scenePlugin->isActive) continue;
					if (scenePlugin->isSleeping) continue;
					scene->draw();
				}
			}

			Amara::Scene* start(std::string key) {
				std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap.find(key);
				if (got != sceneMap.end()) {
					Amara::Scene* scene = sceneMap[key];
					scene->scenePlugin->start();
					return scene;
				}

				return nullptr;
			}

			void manageTasks() {
				Amara::Scene* scene;
				Amara::ScenePlugin* scenePlugin;

				for (size_t i = 0; i < sceneList.size(); i++) {
					scene = sceneList.at(i);
					scenePlugin = scene->scenePlugin;

					scenePlugin->manageTasks();
				}
			}
	};
}

#endif
