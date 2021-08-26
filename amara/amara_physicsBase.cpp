#pragma once
#include "amara.h"

namespace Amara {
	class TilemapLayer;
	
	enum PhysicsShape {
		PHYSICS_RECTANGLE = 0,
		PHYSICS_CIRCLE = 1,
		PHYSICS_LINE = 2,
		PHYSICS_TILEMAP_LAYER = 3
	};

	typedef struct PhysicsProperties {
		FloatRect rect;
		FloatCircle circle;
		FloatLine line;
		TilemapLayer* tilemapLayer;
	} PhysicsProperties;

	class PhysicsBase {
		public:
			Amara::GameProperties* gameProperties = nullptr;
			Entity* parent = nullptr;
			bool deleteWithParent = true;
			std::vector<PhysicsBase*> collisionTargets;

			bool isActive = false;
			bool isDestroyed = false;

			int shape = -1;
			PhysicsProperties properties;

			float x = 0;
			float y = 0;
			float accelerationX = 0;
			float accelerationY = 0;
			float velocityX = 0;
			float velocityY = 0;
			float frictionX = 0;
			float frictionY = 0;

			bool lockedToBounds = false;
			int boundX = 0;
			int boundY = 0;
			int boundW = 0;
			int boundH = 0;
			
			float correctionRate = 0.1;

			int bumpDirections = 0;
			Amara::PhysicsBase* bumped = nullptr;

			bool isPushable = false;
			bool isPushing = false;

			float pushFrictionX = 1;
			float pushFrictionY = 1;

			virtual void create() {}
			virtual void run() {}
			virtual void updateProperties() {}

			virtual bool collidesWith(Amara::Entity* other) {}
			virtual bool collidesWith(Amara::PhysicsBase* other) {}

			virtual bool willCollideWith(Amara::Entity* other) {}
			virtual bool willCollideWith(Amara::PhysicsBase* other) {}

			virtual bool willCollide() {}

			virtual bool hasCollided(bool pushingX, bool pushingY) {}
			bool hasCollided() {
				return hasCollided(false, false);
			}

			virtual void addCollisionTarget(Amara::Entity* other) {}
			virtual void addCollisionTarget(Amara::PhysicsBase* gBody) {
				if (gBody != nullptr) collisionTargets.push_back(gBody);
			}

			void makePushable() {
				isPushable = true;
			}
			void makePushable(float pfx, float pfy) {
				if (pfx == 0 && pfy == 0) {
					isPushable = false;
					return;
				}
				makePushable();
				pushFrictionX = pfx;
				pushFrictionY = pfy;
			}
			void makePushable(float pf) {
				makePushable(pf, pf);
			}

			Amara::PhysicsBase* removeCollisionTarget(Amara::PhysicsBase* gBody) {
				Amara::PhysicsBase* other;
				for (auto it = collisionTargets.begin(); it != collisionTargets.end(); ++it) {
					other = *it;
					if (other == gBody) {
						collisionTargets.erase(it--);
					}
				}
				return gBody;
			}
			virtual Amara::PhysicsBase* removeCollisionTarget(Amara::Entity* other) {}

			void checkActiveCollisionTargets() {
				Amara::PhysicsBase* other;
				for (auto it = collisionTargets.begin(); it != collisionTargets.end(); ++it) {
					other = *it;
					if (other->isDestroyed) {
						collisionTargets.erase(it--);
					}
				}
			}

			void activate() {
				isActive = true;
			}
			void deactivate() {
				isActive = false;
			}

			virtual void destroy() {
				isActive = false;
				isDestroyed = true;
				gameProperties->taskManager->queueDeletion(this);
			}

			void setBounds(int gx, int gy, int gw, int gh) {
				lockedToBounds = true;
				boundX = gx;
				boundY = gy;
				boundW = gw;
				boundH = gh;
			}
			void removeBounds() {
				lockedToBounds = false;
			}
	};
}