#ifndef AMARA_ENTITY
#define AMARA_ENTITY

#include "amara.h"

namespace Amara {
	class Game;
	class Scene;
	class InputManager;
	class ControlScheme;

	class PhysicsBase: public Amara::FloatRect {
		public:
			bool deleteWithParent = true;
			
			virtual void run() {}
	};

	class SortedEntity {
		public:
			int depth = 0;
	};

	struct sortEntities {
		inline bool operator() (Amara::SortedEntity* entity1, Amara::SortedEntity* entity2) {
			return (entity1->depth < entity2->depth);
		}
	};
	
	class Entity : public Amara::SortedEntity, public Amara::Interactable {
		public:
			Amara::GameProperties* properties = nullptr;
			Amara::Game*  game = nullptr;
			Amara::Scene* scene = nullptr;
			Amara::Entity* parent = nullptr;

			Amara::InputManager* input = nullptr;
			Amara::ControlScheme* controls = nullptr;
			Amara::AudioGroup* audio = nullptr;
			Amara::AssetManager* assets = nullptr;
			Amara::Loader* load = nullptr;

			std::vector<Amara::Entity*> entities;

			Amara::PhysicsBase* physics = nullptr;

			std::string id;

			float x = 0;
			float y = 0;
			float z = 0;

			float scaleX = 1;
			float scaleY = 1;

			float scrollFactorX = 1;
			float scrollFactorY = 1;
			float zoomFactorX = 1;
			float zoomFactorY = 1;
			
			float angle = 0;
			float alpha = 1;

			bool isActive = false;
			bool isDestroyed = false;
			bool isVisible = true;

			Entity() {}
			Entity(nlohmann::json config) {
				configure(config);
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
				Amara::Interactable::init(gameProperties);
				
				properties = gameProperties;
				game = properties->game;
				scene = givenScene;
				parent = givenParent;

				input = properties->input;
				controls = properties->controls;
				audio = properties->audio;
				assets = properties->assets;
				load = properties->loader;

				isActive = true;
				create();
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene) {
				init(gameProperties, givenScene, nullptr);
			}

			virtual void configure(nlohmann::json config) {
				if (config.find("id") != config.end()) {
                    id = config["id"];
                }
				if (config.find("x") != config.end()) {
					x = config["x"];
				}
				if (config.find("y") != config.end()) {
					y = config["y"];
				}
				if (config.find("scaleX") != config.end()) {
					scaleX = config["scaleX"];
				}
				if (config.find("scaleY") != config.end()) {
					scaleY = config["scaleY"];
				}
				if (config.find("scrollFactorX") != config.end()) {
					scrollFactorX = config["scrollFactorX"];
				}
				if (config.find("scrollFactorY") != config.end()) {
					scrollFactorY = config["scrollFactorY"];
				}
				if (config.find("isVisible") != config.end()) {
					isVisible = config["isVisible"];
				}
			}

			virtual nlohmann::json toData() {
				nlohmann::json config;
				config["id"] = id;
				config["x"] = x;
				config["y"] = y;
				config["scaleX"] = scaleX;
				config["scaleY"] = scaleY;
				config["scrollFactorX"] = scrollFactorX;
				config["scrollFactorY"] = scrollFactorY;
				config["isVisible"] = isVisible;
				return config;
			}

			Amara::Entity* setId(std::string newId) {
				id = newId;
				return this;
			}

			virtual void draw(int vx, int vy, int vw, int vh) {
				if (properties->quit) return;

				float recScrollX = properties->scrollX * scrollFactorX;
				float recScrollY = properties->scrollY * scrollFactorY;
				float recOffsetX = properties->offsetX + x;
				float recOffsetY = properties->offsetY + y;
				float recZoomX = properties->zoomX * scaleX;
				float recZoomY = properties->zoomY * scaleY;
				float recZoomFactorX = properties->zoomFactorX * zoomFactorX;
				float recZoomFactorY = properties->zoomFactorY * zoomFactorY;
				float recAngle = properties->angle + angle;

				if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;
				
				stable_sort(entities.begin(), entities.end(), sortEntities());
				
				Amara::Entity* entity;
				for (auto it = entities.begin(); it != entities.end(); it++) {
                    entity = *it;

                    if (entity->isDestroyed || entity->parent != this) {
                        entities.erase(it--);
                        continue;
                    }
					if (!entity->isVisible) continue;
					
					properties->scrollX = recScrollX;
					properties->scrollY = recScrollY;
					properties->offsetX = recOffsetX;
					properties->offsetY = recOffsetY;
					properties->zoomX = recZoomX;
					properties->zoomY = recZoomY;
					properties->zoomFactorX = recZoomFactorX;
					properties->zoomFactorY = recZoomFactorY;
					properties->angle = recAngle;
					entity->draw(vx, vy, vw, vh);
                }
			}

			virtual void run() {
				Amara::Interactable::run();
				update();
				if (physics != nullptr) {
					physics->run();
				}

				for (Amara::Entity* entity : entities) {
					if (entity->isDestroyed || entity->parent != this) continue;
					entity->run();
				}
			}

			virtual Amara::Entity* get(std::string find) {
				for (Amara::Entity* entity : entities) {
					if (entity->id.compare(find) == 0) {
						return entity;
					} 
				}
				return nullptr;
			}

			virtual Amara::Entity* add(Amara::Entity* entity) {
				entities.push_back(entity);
				entity->init(properties, scene, this);
				return entity;
			}

			virtual Amara::Entity* remove(size_t index) {
				Amara::Entity* child = entities.at(index);
				child->parent = nullptr;
				entities.erase(entities.begin() + index);
				return child;
			}

			virtual Amara::Entity* remove(Amara::Entity* entity) {
				Amara::Entity* child;
				int numChildren = entities.size();
				for (size_t i = 0; i < numChildren; i++) {
					child = entities.at(i);
					if (child == entity) {
						return remove(i);
					}
				}
				return nullptr;
			}

			virtual Amara::Entity* add(Amara::PhysicsBase* gPhysics) {
				if (physics != nullptr) {
					delete physics;
				}
				physics = gPhysics;
			}

			virtual void remove(Amara::PhysicsBase* fPhysics) {
				if (physics == fPhysics) {
					delete physics;
					physics = nullptr;
				}
			}

			virtual void destroy(bool recursiveDestroy) {
				parent = nullptr;

				Amara::Entity* child;
				int numChildren = entities.size();
				for (size_t i = 0; i < numChildren; i++) {
					child = entities.at(i);
					if (recursiveDestroy) {
						child->destroy();
					}
					else {
						child->parent = nullptr;
					}
				}
				if (!recursiveDestroy) {
					entities.clear();
				}

				isDestroyed = true;
				isActive = false;

				properties->taskManager->queueDeletion(this);
			}

			virtual void destroy() {
				destroy(true);
			}

			void setVisible(bool val) {
				isVisible = val;
			}

			void setScale(float gx, float gy) {
                scaleX = gx;
                scaleY = gy;
            }
            void setScale(float g) {
                setScale(g, g);
            }

            void changeScale(float gx, float gy) {
                scaleX += gx;
                scaleY += gy;
            }
            void changeScale(float gi) {
                changeScale(gi, gi);
            }

			void setScrollFactor(float gx, float gy) {
				scrollFactorX = gx;
				scrollFactorY = gy;
			}
			void setScrollFactor(float gi) {
				setScrollFactor(gi, gi);
			}

			void setZoomFactor(float gx, float gy) {
				zoomFactorX = gx;
				zoomFactorY = gy;
			}
			void setZoomFactor(float gi) {
				setZoomFactor(gi, gi);
			}

			virtual void bringToFront() {
				std::vector<Amara::Entity*>& rSceneEntities = parent->entities;
				for (Amara::Entity* entity: rSceneEntities) {
					if (depth <= entity->depth) {
						depth = entity->depth + 1;
					}
				}
			}

			virtual void setLoader(Amara::Loader* gLoader, bool recursive) {
				load = gLoader;
				if (recursive) {
					for (Amara::Entity* entity: entities) {
						entity->setLoader(gLoader);
					}
				}
			}

			virtual void setLoader(Amara::Loader* gLoader) {
				setLoader(gLoader, true);
			}

			virtual void create() {}
			virtual void update() {}

			virtual ~Entity() {
				if (physics != nullptr && physics->deleteWithParent) {
					delete physics;
				}
			}
		protected:
	};
}
#endif
