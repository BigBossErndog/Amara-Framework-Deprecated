#ifndef AMARA_ENTITY
#define AMARA_ENTITY

#include "amara.h"

namespace Amara {
	class Scene;
	class Entity;

	class SortedEntity {
		public:
			int depth = 0;
	};

	struct sortEntities {
		inline bool operator() (Amara::SortedEntity* entity1, Amara::SortedEntity* entity2) {
			return (entity2->depth - entity1->depth);
		}
	};
	
	class Entity : public Amara::SortedEntity  {
		public:
			Amara::GameProperties* properties;
			Amara::Scene* scene = nullptr;
			Amara::Entity* parent = nullptr;
			vector<Amara::Entity*> entities;

			string id;

			float x = 0;
			float y = 0;

			bool isActive = false;
			bool destroyed = false;

			Entity() {
				
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
				properties = gameProperties;
				scene = givenScene;
				parent = givenParent;
				isActive = true;
				create();
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene) {
				init(gameProperties, givenScene, nullptr);
			}

			virtual void draw(int vx, int vy, int vw, int vh) {
				stable_sort(entities.begin(), entities.end(), sortEntities());

				Amara::Entity* entity;
				for (size_t i = 0; i < entities.size(); i++) {
					entity = entities.at(i);
					entity->draw(vx, vy, vw, vh);
				}
			}

			virtual void run() {
				update();

				Amara::Entity* entity;
				for (size_t i = 0; i < entities.size(); i++) {
					entity = entities.at(i);
					entity->run();
				}
			}

			Amara::Entity* get(string find) {
				Amara::Entity* entity;
				for (size_t i = 0; i < entities.size(); i++) {
					entity = entities.at(i);
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
				for (size_t i = 0; i < entities.size(); i++) {
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
				for (size_t i = 0; i < entities.size(); i++) {
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

				destroyed = true;
				isActive = false;
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
