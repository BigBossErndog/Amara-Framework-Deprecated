namespace Amara {
	class TilemapLayer;
	
	enum PhysicsShape {
		PHYSICS_RECTANGLE = 0,
		PHYSICS_CIRCLE = 1,
		PHYSICS_LINE = 2,
		PHYSICS_TILEMAP_LAYER = 3
	};

	class PhysicsProperties {
	public:
		int shape = -1;
		FloatRect rect;
		FloatCircle circle;
		FloatLine line;
		TilemapLayer* tilemapLayer;

		void setRect(FloatRect gRect) {
			shape = PHYSICS_RECTANGLE;
			rect = gRect;
		}
		void setRect(float gx, float gy, float gw, float gh) {
			setRect({ gx, gy, gw, gh });
		}
		static PhysicsProperties newRect(FloatRect gRect) {
			PhysicsProperties nRect;
			nRect.setRect(gRect);
			return nRect;
		}
		static PhysicsProperties newRect(float gx, float gy, float gw, float gh) {
			return newRect({ gx, gy, gw, gh });
		}

		void setCircle(FloatCircle gCircle) {
			shape = PHYSICS_CIRCLE;
			circle = gCircle;
		}
		void setCircle(float gx, float gy, float gr) {
			setCircle({ gx, gy, gr });
		}
		static PhysicsProperties newCircle(FloatCircle gCircle) {
			PhysicsProperties nCircle;
			nCircle.setCircle(gCircle);
			return nCircle;
		}
		static PhysicsProperties newCircle(float gx, float gy, float gr) {
			return newCircle({ gx, gy, gr });
		}

		void setLine(FloatLine gLine) {
			shape = PHYSICS_LINE;
			line = gLine;
		}
		void setLine(float x1, float y1, float x2, float y2) {
			setLine({ x1, y1, x2, y2 });
		}
		static PhysicsProperties newLine(FloatLine gLine) {
			PhysicsProperties nLine;
			nLine.setLine(gLine);
			return nLine;
		}
		static PhysicsProperties newLine(float x1, float y1, float x2, float y2) {
			return newLine(x1, y1, x2, y2);
		}

		void adjustXY(float gx, float gy) {
			switch (shape) {
				case PHYSICS_RECTANGLE:
					rect.x += gx;
					rect.y += gy;
					break;
			}
		}
	};

	class PhysicsBase: public FloatVector2 {
		public:
			Amara::GameProperties* gameProperties = nullptr;
			Entity* parent = nullptr;
			bool deleteWithParent = true;
			std::vector<PhysicsBase*> collisionTargets;

			bool isActive = false;
			bool isPaused = false;
			bool isDestroyed = false;
			bool isWall = true;
			bool isColliding = true;

			PhysicsProperties properties;

			FloatVector2 acceleration = { 0, 0 };
			FloatVector2 velocity = { 0, 0 };
			FloatVector2 friction = { 0, 0 };
			FloatVector2 bounce = { 0, 0 };

			bool lockedToBounds = false;
			IntRect bounds = { 0, 0, 0, 0 };
			
			float correctionRate = 0.2;
			int correctionTries = 500;

			bool useDeltaTime = true;
			
			int bumpDirections = 0;
			Amara::PhysicsBase* bumped = nullptr;

			bool isPushable = false;
			bool isPushing = false;

			FloatVector2 pushFriction = { 1, 1 };

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
				if (gBody != nullptr && gBody != this) collisionTargets.push_back(gBody);
			}
			virtual void addCollisionTargets(std::vector<Amara::Entity*> others) {
				for (Amara::Entity* other: others) {
					addCollisionTarget(other);
				}
			}
			virtual void addCollisionTargets(std::vector<Amara::PhysicsBase*> others) {
				for (Amara::PhysicsBase* other: others) {
					addCollisionTarget(other);
				}
			}

			void setVelocity(float gx, float gy) {
				velocity.x = gx;
				velocity.y = gy;
			}
			void setVelocity(float gv) {
				setVelocity(gv, gv);
			}

			void setAcceleration(float gx, float gy) {
				acceleration.x = gx;
				acceleration.y = gy;
			}
			void setAcceleration(float ga) {
				setAcceleration(ga, ga);
			} 
			
			void setFriction(float gx, float gy) {
				friction.x = gx;
				friction.y = gy;
			}
			void setFriction(float gf) {
				setFriction(gf, gf);
			}

			void setBounce(float gx, float gy) {
				bounce.x = gx;
				bounce.y = gy;
			}
			void setBounce(float gb) {
				setBounce(gb, gb);
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
				pushFriction.x = pfx;
				pushFriction.y = pfy;
			}
			void makePushable(float pf) {
				makePushable(pf, pf);
			}

			Amara::PhysicsBase* removeCollisionTarget(Amara::PhysicsBase* gBody) {
				Amara::PhysicsBase* other;
				for (auto it = collisionTargets.begin(); it != collisionTargets.end();) {
					other = *it;
					if (other == gBody) {
						it = collisionTargets.erase(it);
						continue;
					}
					++it;
				}
				return gBody;
			}
			virtual Amara::PhysicsBase* removeCollisionTarget(Amara::Entity* other) {}

			void checkActiveCollisionTargets();

			void activate() {
				isActive = true;
			}
			void deactivate() {
				isActive = false;
			}

			virtual void destroy() {
				if (isDestroyed) return;
				isActive = false;
				isDestroyed = true;
				gameProperties->taskManager->queuePhysics(this);
			}

			void setBounds(int gx, int gy, int gw, int gh) {
				lockedToBounds = true;
				bounds.x = gx;
				bounds.y = gy;
				bounds.width = gw;
				bounds.height = gh;
			}
			void removeBounds() {
				lockedToBounds = false;
			}

			void pause(bool gPause) {
				isPaused = gPause;
			}
			void pause() {
				pause(true);
			}
			void unpause() {
				pause(false);
			}

			virtual void correctCollision() {}
	};

	void Amara::TaskManager::queuePhysics(Amara::PhysicsBase* body) {
		if (body == nullptr) return;
		body->isDestroyed = true;
		physicsBuffer.push_back(body);
	}
}