namespace Amara {
	class Game;

	class SceneManager {
		public:
			Amara::Game* game = nullptr;
			Amara::GameProperties* properties = nullptr;
			std::unordered_map<std::string, Amara::Scene*> sceneMap;
			std::vector<Amara::Scene*> sceneList;
			std::vector<Amara::Scene*> copySceneList;

			SceneManager(Amara::GameProperties* gameProperties) {
				properties = gameProperties;
				game = properties->game;
			}

			Amara::Scene* add(std::string key, Amara::Scene* scene, bool willStart) {
				sceneMap[key] = scene;
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
				Amara::ScenePlugin* scenes;

				for (size_t i = 0; i < sceneList.size(); i++) {
					scene = sceneList.at(i);
					scenes = scene->scenes;

					if (!scenes->isActive) continue;
					if (scenes->isPaused) continue;
					if (scenes->isSleeping) continue;

					scene->run();
				}
			}

			void draw() {
				Amara::Scene* scene;
				Amara::ScenePlugin* scenes;

				for (size_t i = 0; i < sceneList.size(); i++) {
					scene = sceneList.at(i);
					scenes = scene->scenes;

					if (!scenes->isActive) continue;
					if (scenes->isSleeping) continue;
					scene->draw();
				}
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
				Amara::ScenePlugin* scenes;
				copySceneList = sceneList;

				for (size_t i = 0; i < copySceneList.size(); i++) {
					scene = copySceneList.at(i);
					scenes = scene->scenes;

					scenes->manageTasks();
				}
			}

			void checkChildren() {
				Amara::Scene* scene;
				for (size_t i = 0; i < sceneList.size(); i++) {
					scene = sceneList.at(i);
					scene->checkChildren(true);
				}
			}
	};
}
