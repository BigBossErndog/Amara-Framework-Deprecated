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
			vector<Amara::Entity*> entities;

			Entity() {
				
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene) {
				properties = gameProperties;
				scene = givenScene;
				create();
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

			virtual ~Entity() {
				Amara::Entity* entity;
                for (size_t i = 0; i < entities.size(); i++) {
                    entity = entities.at(i);
                    delete entity;
                }
                entities.clear();
			}

			virtual void create() {}
			virtual void update() {}
		protected:
	};
}
#endif
