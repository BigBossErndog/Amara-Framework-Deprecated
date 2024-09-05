namespace Amara {
	class Game;

	class SceneManager {
		public:
			Amara::Game* game = nullptr;
			Amara::GameProperties* properties = nullptr;
			std::unordered_map<std::string, Amara::Scene*> sceneMap;
			std::list<Amara::Scene*> sceneList;
			std::list<Amara::Scene*> copySceneList;

			SceneManager() {}
			SceneManager(Amara::GameProperties* gameProperties) {
				properties = gameProperties;
				game = properties->game;
			}

			Amara::Scene* add(std::string key, Amara::Scene* scene, bool willStart) {
				sceneMap[key] = scene;
				scene->depth = sceneList.size()*0.001;
				sceneList.push_back(scene);
				scene->setup(properties, new ScenePlugin(key, properties, scene, &sceneMap, &sceneList));
				scene->key = key;
				if (properties->testing) SDL_Log("ADDED SCENE: %s", scene->scenes->key.c_str());
				if (willStart) scene->scenes->start();
				return scene;
			}

			Amara::Scene* add(std::string key, Amara::Scene* scene) {
				return add(key, scene, false);
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

				for (auto it = sceneList.begin(); it != sceneList.end();) {
					scene = *it;
					scenePlugin = scene->scenes;

					if (properties->reloadAssets) {
						scenePlugin->reloadAssets = true;
					}

					if (!scenePlugin->isActive || scenePlugin->isPaused || scenePlugin->isSleeping) {
						++it;
						continue;
					}
					scene->run();
					++it;
				}
			}

			void draw() {
				Amara::Scene* scene;
				Amara::ScenePlugin* scenePlugin;

				sceneList.sort(sortEntitiesByDepth());

				properties->inSceneDrawing = true;
				bool recReloadAssets = properties->reloadAssets;
				for (auto it = sceneList.begin(); it != sceneList.end();) {
					scene = *it;
					scenePlugin = scene->scenes;

					if (!scenePlugin->isActive || scenePlugin->isSleeping) {
						++it;
						continue;
					}
					properties->reloadAssets = recReloadAssets || scenePlugin->reloadAssets;
					scene->draw();
					scenePlugin->reloadAssets = false;
					properties->reloadAssets = recReloadAssets;
					++it;
				}
				properties->inSceneDrawing = false;
			}

			Amara::Scene* get(std::string key) {
				std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap.find(key);
				if (got != sceneMap.end()) {
					return sceneMap[key];
				}
				return nullptr;
			}

			bool has(std::string key) {
				return get(key) != nullptr;
			}
			
			Amara::Scene* start(std::string key) {
				std::unordered_map<std::string, Amara::Scene*>::iterator got = sceneMap.find(key);
				if (got != sceneMap.end()) {
					Amara::Scene* scene = sceneMap[key];
					scene->scenes->start();
					return scene;
				}
				else {
					SDL_Log("Scene with key \"%s\" not found. Could not be started.", key.c_str());
				}

				return nullptr;
			}

			void manageTasks() {
				Amara::Scene* scene;
				Amara::ScenePlugin* scenePlugin;
				copySceneList = sceneList;

				for (auto it = copySceneList.begin(); it != copySceneList.end();) {
					scene = *it;
					scenePlugin = scene->scenes;
					scenePlugin->manageTasks();
					++it;
				}
			}

			void checkChildren() {
				Amara::Scene* scene;
				for (auto it = sceneList.begin(); it != sceneList.end();) {
					scene = *it;
					scene->checkChildren(true);
					++it;
				}
			}
	};
}
