#ifndef AMARA_ENTITY
#define AMARA_ENTITY

#include "amara.h"

namespace Amara {
	class Scene;
	class InputManager;
	class ControlScheme;

	class SortedEntity {
		public:
			int depth = 0;
	};

	struct sortEntities {
		inline bool operator() (Amara::SortedEntity* entity1, Amara::SortedEntity* entity2) {
			return (entity1->depth < entity2->depth);
		}
	};
	
	class Entity : public Amara::SortedEntity, public Amara::Interactable  {
		public:
			Amara::GameProperties* properties;
			Amara::Scene* scene = nullptr;
			Amara::Entity* parent = nullptr;

			Amara::InputManager* input = nullptr;
			Amara::ControlScheme* controls = nullptr;
			Amara::AudioGroup* audio = nullptr;
			Amara::Loader* load = nullptr;

			vector<Amara::Entity*> entities;

			string id;

			float x = 0;
			float y = 0;
			
			float angle = 0;
			float alpha = 1;

			bool isActive = false;
			bool isDestroyed = false;

			Entity() {
				
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
				Amara::Interactable::init(gameProperties);
				properties = gameProperties;
				scene = givenScene;
				parent = givenParent;

				input = properties->input;
				controls = properties->controls;
				audio = properties->audio;
				load = properties->loader;

				isActive = true;
				create();
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene) {
				init(gameProperties, givenScene, nullptr);
			}

			Amara::Entity* setId(string newId) {
				id = newId;
				return this;
			}

			virtual void draw(int vx, int vy, int vw, int vh) {
				float recScrollX = properties->scrollX;
				float recScrollY = properties->scrollY;

				if (alpha < 0) alpha = 0;
                if (alpha > 1) alpha = 1;
				
				stable_sort(entities.begin(), entities.end(), sortEntities());

				for (Amara::Entity* entity : entities) {
					properties->scrollX = recScrollX + x;
					properties->scrollY = recScrollY + y;
					if (entity->isDestroyed || entity->parent != this) continue;
					entity->draw(vx, vy, vw, vh);
				}
			}

			virtual void run() {
				Amara::Interactable::run();
				update();

				for (Amara::Entity* entity : entities) {
					if (entity->isDestroyed || entity->parent != this) continue;
					entity->run();
				}
			}

			Amara::Entity* get(string find) {
				for (Amara::Entity* entity : entities) {
					if (entity->id.compare(find) == 0) {
						return entity;
					} 
				}
				return nullptr;
			}

			Amara::Entity* add(Amara::Entity* entity) {
				entities.push_back(entity);
				entity->init(properties, scene);
				return entity;
			}

			Amara::Entity* remove(size_t index) {
				Amara::Entity* child = entities.at(index);
				child->parent = nullptr;
				entities.erase(entities.begin() + index);
				return child;
			}

			Amara::Entity* remove(Amara::Entity* entity) {
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

			void destroy(bool recursiveDestroy) {
				if (parent != nullptr) {
					parent->remove(this);
				}

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

			void destroy() {
				destroy(true);
			}

			virtual void create() {}
			virtual void update() {}
		protected:
	};
}
#endif
