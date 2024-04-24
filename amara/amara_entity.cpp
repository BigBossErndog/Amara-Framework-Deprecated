namespace Amara {
	class Game;
	class Scene;
	class InputManager;
	class ControlScheme;
	class TilemapLayer;
	class Entity;

	class SortedEntity {
	public:
		float depth = 0;
	};

	struct sortEntitiesByDepth {
		inline bool operator() (Amara::SortedEntity* entity1, Amara::SortedEntity* entity2) {
			if (entity1 == nullptr) return false;
			if (entity2 == nullptr) return false;
			return (entity1->depth < entity2->depth);
		}
	};

	class Entity : public Amara::SortedEntity, public Amara::Interactable, public Amara::Broadcaster, public Amara::FloatVector3, public Amara::DataObject {
	public:
		Amara::Game*  game = nullptr;
		Amara::Scene* scene = nullptr;
		Amara::Entity* parent = nullptr;

		Amara::Entity* attachedTo = nullptr;
		float attachmentOffsetX = 0;
		float attachmentOffsetY = 0;

		Amara::ControlScheme* controls = nullptr;
		Amara::AudioGroup* audio = nullptr;
		Amara::AssetManager* assets = nullptr;
		Amara::TaskManager* tasks = nullptr;
		Amara::Loader* load = nullptr;

		std::list<Amara::Entity*> children;

		Amara::PhysicsBase* physics = nullptr;

		std::string id;
		std::string entityType;

		float scaleX = 1;
		float scaleY = 1;

		float cameraOffsetX = 0;
		float cameraOffsetY = 0;
		
		float scrollFactorX = 1;
		float scrollFactorY = 1;
		float zoomFactorX = 1;
		float zoomFactorY = 1;

		double angle = 0; // Using Degrees
		float alpha = 1;

		bool isActive = false;
		bool isDestroyed = false;
		bool isVisible = true;
		bool isPaused = false;

		bool shouldSortChildren = true;
		bool sortChildrenOnce = false;

		int sortingDelay = 0;
		int sortingDelayCounter = 0;

		static bool debuggingDefault;
		std::string debugID;
		bool debugging = debuggingDefault;

		std::vector<Amara::Entity*> entityBuffer;
		bool runningEntities = false;

		Entity() {}
		Entity(Amara::GameProperties* gProperties) {
			Amara::Entity::init(gProperties);
		}

		virtual void init(Amara::GameProperties* gameProperties) {
			Amara::Interactable::init(gameProperties);

			game = properties->game;
			controls = properties->controls;
			audio = properties->audio;
			assets = properties->assets;
			load = properties->loader;
			messages = properties->messages;
			tasks = properties->taskManager;

			isActive = true;
			entityType = "entity";
		}
		virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene, Amara::Entity* givenParent) {
			Amara::Entity::init(gameProperties);

			scene = givenScene;
			parent = givenParent;
			
			init();
			preload();
			if (!isDestroyed) create();
		}

		virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene) {
			init(gameProperties, givenScene, nullptr);
		}

		virtual void init() {}

		using Amara::DataObject::configure;
		virtual void configure(nlohmann::json config) {
			if (config.find("id") != config.end()) {
				id = config["id"];
			}
			if (config.find("entityType") != config.end()) {
				entityType = config["entityType"];
			}

			if (config.find("x") != config.end()) x = config["x"];
			if (config.find("y") != config.end()) y = config["y"];
			if (config.find("z") != config.end()) z = config["z"];

			if (config.find("xFromRight") != config.end()) {
				int xFromRight = config["xFromRight"];
				x = properties->resolution->width - xFromRight;
			}
			if (config.find("yFromBottom") != config.end()) {
				int yFromBottom = config["yFromBottom"];
				y = properties->resolution->height - yFromBottom;
			}
			if (config.find("xFromCenter") != config.end()) {
				int xFromCenter = config["xFromCenter"];
				x = properties->resolution->width/2.0 + xFromCenter;
			}
			if (config.find("yFromCenter") != config.end()) {
				int yFromCenter = config["yFromCenter"];
				y = properties->resolution->height/2.0 + yFromCenter;
			}

			if (config.find("relativeX") != config.end()) {
				float relativeX = config["relativeX"];
				x = properties->resolution->width * relativeX;
			}
			if (config.find("relativeY") != config.end()) {
				float relativeY = config["relativeY"];
				y = properties->resolution->height * relativeY;
			}

			if (config.find("relativeXFromCenter") != config.end()) {
				float relativeX = config["relativeXFromCenter"];
				x = properties->resolution->width/2.0 + properties->resolution->width*relativeX/2.0;
			}
			if (config.find("relativeYFromCenter") != config.end()) {
				float relativeY = config["relativeYFromCenter"];
				y = properties->resolution->height/2.0 + properties->resolution->height*relativeY/2.0;
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
			if (config.find("scrollFactor") != config.end()) {
				scrollFactorX = config["zoomFactor"];
				scrollFactorY = config["zoomFactor"];
			}
			if (config.find("zoomFactorX") != config.end()) {
				zoomFactorX = config["zoomFactorX"];
			}
			if (config.find("zoomFactorY") != config.end()) {
				zoomFactorY = config["zoomFactorY"];
			}
			if (config.find("zoomFactor") != config.end()) {
				zoomFactorX = config["zoomFactor"];
				zoomFactorY = config["zoomFactor"];
			}
			if (config.find("isVisible") != config.end()) {
				isVisible = config["isVisible"];
			}
			if (config.find("angle") != config.end()) {
				angle = config["angle"];
			}
			if (config.find("alpha") != config.end()) {
				alpha = config["alpha"];
			}
			if (config.find("depth") != config.end()) {
				depth = config["depth"];
			}
			if (config.find("cameraOffsetX") != config.end()) {
				cameraOffsetX = config["cameraOffsetX"];
			}
			if (config.find("cameraOffsetY") != config.end()) {
				cameraOffsetY = config["cameraOffsetY"];
			}
			if (config.find("data") != config.end() && !config["data"].is_null()) {
				data.update(config["data"]);
			}
			if (config.find("bringToFront") != config.end()) {
				if (config["bringToFront"]) {
					bringToFront();
				}
			}
			if (config.find("sendToBack") != config.end()) {
				if (config["sendToBack"]) {
					sendToBack();
				}
			}
			if (config.find("isInteractable") != config.end()) {
				setInteractable(config["isInteractable"]);
			}
			if (config.find("isDraggable") != config.end()) {
				setDraggable(config["isDraggable"]);
			}
		}
		void recursiveConfigure(nlohmann::json config) {
			configure(config);
			for (Amara::Entity* child: children) {
				child->recursiveConfigure(config);
			}
		}

		virtual nlohmann::json toData() {
			nlohmann::json config;
			config["id"] = id;
			config["entityType"] = entityType;
			config["x"] = x;
			config["y"] = y;
			config["z"] = z;
			config["scaleX"] = scaleX;
			config["scaleY"] = scaleY;
			config["scrollFactorX"] = scrollFactorX;
			config["scrollFactorY"] = scrollFactorY;
			config["zoomFactorX"] = zoomFactorX;
			config["zoomFactorY"] = zoomFactorY;
			config["isVisible"] = isVisible;
			config["alpha"] = alpha;
			config["depth"] = depth;
			config["cameraOffsetX"] = cameraOffsetX;
			config["cameraOffsetY"] = cameraOffsetY;
			config["numberOfChildren"] = children.size();
			config["data"] = data;
			return config;
		}
		
		Amara::Entity* setId(std::string newId) {
			id = newId;
			return this;
		}

		Amara::Entity* sortChildren() {
			children.sort(sortEntitiesByDepth());
			return this;
		}
		Amara::Entity* delayedSorting() {
			sortingDelayCounter += 1;
			if (sortingDelayCounter >= sortingDelay) {
				sortChildren();
				sortingDelayCounter = 0;
			}
			return this;
		}

		Amara::Entity* setChildrenSorting(bool always) {
			shouldSortChildren = always;
			return this;
		}
		Amara::Entity* setChildrenSorting(bool always, bool once) {
			setChildrenSorting(always);
			sortChildrenOnce = once;
			return this;
		}

		Amara::Entity* sortOnce() {
			setChildrenSorting(false, true);
			return this;
		}

		virtual void draw(int vx, int vy, int vw, int vh) {
			if (properties->quit) return;
			if (physics) {
				physics->checkActiveCollisionTargets();
			}

			if (alpha < 0) alpha = 0;
			if (alpha > 1) alpha = 1;

			float recScrollX = properties->scrollX * scrollFactorX;
			float recScrollY = properties->scrollY * scrollFactorY;
			float recOffsetX = properties->offsetX + x;
			float recOffsetY = properties->offsetY + y;
			float recZoomX = properties->zoomX * scaleX;
			float recZoomY = properties->zoomY * scaleY;
			float recZoomFactorX = properties->zoomFactorX * zoomFactorX;
			float recZoomFactorY = properties->zoomFactorY * zoomFactorY;
			float recAngle = properties->angle + angle;
			float recAlpha = properties->alpha * alpha;
			
			if (shouldSortChildren || sortChildrenOnce) {
				sortChildrenOnce = false;
				delayedSorting();
			}

			Amara::Entity* entity;
			for (auto it = children.begin(); it != children.end();) {
				entity = *it;

				if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
					if (properties->inSceneDrawing) it = children.erase(it);
					else ++it;
					continue;
				}
				if (entity->isVisible) {
					properties->scrollX = recScrollX;
					properties->scrollY = recScrollY;
					properties->offsetX = recOffsetX;
					properties->offsetY = recOffsetY;
					properties->zoomX = recZoomX;
					properties->zoomY = recZoomY;
					properties->zoomFactorX = recZoomFactorX;
					properties->zoomFactorY = recZoomFactorY;
					properties->angle = recAngle;
					properties->alpha = recAlpha;
					entity->draw(vx, vy, vw, vh);
				}
				++it;
			}
		}

		virtual void drawToTexture(SDL_Texture* tx, int gx, int gy) {
			SDL_Texture* recTarget = SDL_GetRenderTarget(properties->gRenderer);
			SDL_SetRenderTarget(properties->gRenderer, tx);
			float recX = x, recY = y;
			int vw, vh;
			x = gx;
			y = gy;

			SDL_QueryTexture(tx, NULL, NULL, &vw, &vh);
			draw(0, 0, vw, vh);

			SDL_SetRenderTarget(properties->gRenderer, recTarget);
			x = recX;
			y = recY;
		}
		void drawToTexture(SDL_Texture* tx) {
			drawToTexture(tx, x, y);
		}

		Amara::Entity* goToXY(float gx, float gy) {
			x = gx; y = gy;
			return this;
		}
		Amara::Entity* goToXY(FloatVector2 v) { return goToXY(v.x, v.y); }
		Amara::Entity* goToXY(IntVector2 v) { return goToXY(v.x, v.y); }

		Amara::Entity* goToXYZ(float gx, float gy, float gz) {
			goToXY(gx, gy);
			x = gz;
			return this;
		}
		Amara::Entity* goToXY(FloatVector3 v) { return goToXYZ(v.x, v.y, v.z); }
		Amara::Entity* goToXY(IntVector3 v) { return goToXYZ(v.x, v.y, v.z); }

		Amara::Entity* pause(bool p) {
			isPaused = p;
			return this;
		}
		Amara::Entity* pause() { return pause(true); }
		Amara::Entity* unpause() { return pause(false); }

		virtual void run() {
			debugID = id;
			std::string debugCopy;
			if (debugging) {
				debugID = "";
				for (int i = 0; i < properties->entityDepth; i++) debugID += "\t";
				debugID += id;
				SDL_Log("%s (%s): Running. - depth %d", debugID.c_str(), entityType.c_str(), properties->entityDepth);
				debugCopy = debugID;
			}
			
			receiveMessages();
			updateMessages();
			if (isDestroyed) return;

			Amara::Interactable::run();
			if (isInteractable && interact.isDraggable && interact.isDown) {
				interact.isBeingDragged = true;
				if (physics) {
					physics->velocityX = interact.movementX;
					physics->velocityY = interact.movementY;
				}
				else {
					x += interact.movementX;
					y += interact.movementY;
				}
			}
			else interact.isBeingDragged = false;

			update();
			if (isDestroyed) return;

			if (physics != nullptr) {
				if (physics->isActive) physics->run();

				if (physics->isDestroyed) {
					removePhysics();
				}
			}

			if (attachedTo != nullptr) {
				if (attachedTo->isDestroyed) {
					attachedTo = nullptr;
				}
				else {
					x = attachedTo->x + attachmentOffsetX;
					y = attachedTo->y + attachmentOffsetY;
				}
			}
			
			runChildren();

			if (debugging) SDL_Log("%s (%s): Finished Running.", debugCopy.c_str(), entityType.c_str());
		}

		virtual void runChildren() {
			if (isDestroyed) return;
			Amara::Entity* entity;
			properties->entityDepth += 1;
			runningEntities = true;
			if (debugging) {
				debugID = "";
				for (int i = 0; i < properties->entityDepth; i++) debugID += "\t";
				debugID += id;
			}
			for (auto it = children.begin(); it != children.end();) {
				entity = *it;
				if (entity == nullptr || entity->isDestroyed || entity->parent != this || entity->isPaused) {
					++it;
					continue;
				}
				if (debugging) SDL_Log("%s (%s): Running Child %d \"%s\" - depth %d", debugID.c_str(), entityType.c_str(), std::distance(it, children.begin()), entity->id.c_str(), properties->entityDepth);
				entity->run();
				++it;
				if (isDestroyed) break;
			}
			runningEntities = false;
			if (!isDestroyed) pipeEntityBuffer();
			properties->entityDepth -= 1;
		}

		virtual Amara::Entity* get(std::string find) {
			if (find.size() == 0) return nullptr;
			Amara::Entity* entity;
			for (auto it = children.begin(); it != children.end();) {
				entity = *it;
				++it;
				if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
					continue;
				}
				if (entity->id.compare(find) == 0) {
					return entity;
				}
			}
			for (auto it = entityBuffer.begin(); it != entityBuffer.end();) {
				entity = *it;
				++it;
				if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
					continue;
				}
				if (entity->id.compare(find) == 0) {
					return entity;
				}
			}
			return nullptr;
		}

		/**
		 * Do not use this within its own run loop.
		*/
		virtual Amara::Entity* add(Amara::Entity* entity) {
			if (entity == nullptr || entity->isDestroyed) return nullptr;
			if (entity->parent) {
				return entity->setParent(this);
			}
			else {
				if (runningEntities) {
					entityBuffer.push_back(entity);
				}
				else children.push_back(entity);
			}
			entity->init(properties, scene, this);
			return entity;
		}

		Amara::Entity* remove(Amara::Entity* entity) {
			Amara::Entity* child;
			for (auto it = children.begin(); it != children.end();) {
				child = *it;
				if (child == entity) {
					if (entity->parent == this) entity->parent = nullptr;
				}
				++it;
			}
			for (auto it = entityBuffer.begin(); it != entityBuffer.end();) {
				child = *it;
				if (child == entity) {
					if (entity->parent == this) entity->parent = nullptr;
				}
				++it;
			}
			return this;
		}

		Amara::Entity* removeEntities() {
			for (Amara::Entity* entity: children) {
				if (entity->parent == this) entity->parent = nullptr;
			}
			children.clear();
			return this;
		}

		/**
		 * Do not use this within its own run loop or parent's run loop.
		*/
		Amara::Entity* setParent(Entity* newParent) {
			if (isDestroyed) return this;
			if (parent && !parent->isDestroyed) parent->remove(this);
			parent = newParent;
			parent->children.push_back(this);
			return this;
		}

		void checkChildren(bool recursive) {
			Amara::Entity* entity;
			for (auto it = children.begin(); it != children.end();) {
				entity = *it;
				if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
					it = children.erase(it);
					continue;
				}
				if (recursive) entity->checkChildren(true);
				++it;
			}
		}
		void checkChildren() {
			checkChildren(false);
		}

		void pipeEntityBuffer() {
			for (Amara::Entity* entity: entityBuffer) {
				children.push_back(entity);
			}
			entityBuffer.clear();
		}

		virtual Amara::PhysicsBase* addPhysics(Amara::PhysicsBase* gPhysics) {
			physics = gPhysics;
			physics->isActive = true;
			physics->parent = this;
			physics->gameProperties = properties;
			physics->updateProperties();
			physics->create();
			return physics;
		}

		virtual Amara::PhysicsBase* removePhysics() {
			Amara::PhysicsBase* rec = physics;
			if (physics) {
				physics = nullptr;
			}
			return rec;
		}
		virtual void destroyPhysics() {
			Amara::PhysicsBase* rec = removePhysics();
			if (rec) {
				rec->destroy();
			}
		}

		void destroyChildren() {
			for (Amara::Entity* child: children) {
				child->destroy();
			}
			children.clear();
		}

		virtual void destroyEntities(bool recursiveDestroy) {
			Amara::Entity* child = nullptr;
			for (auto it = children.begin(); it != children.end();) {
				child = *it;
				if (child == nullptr || child->isDestroyed || child->parent != this) {
					++it;
					continue;
				}
				if (recursiveDestroy) {
					child->destroy();
				}
				else {
					child->parent = nullptr;
				}
				++it;
			}
			children.clear();
		}

		virtual void destroyEntities() {
			destroyEntities(true);
		}

		virtual void destroy(bool recursiveDestroy) {
			if (isDestroyed) return;
			parent = nullptr;
			destroyEntities(recursiveDestroy);
			isDestroyed = true;
			isActive = false;
			properties->taskManager->queueDeletion(this);
			if (physics) {
				physics->destroy();
				physics = nullptr;
			}
		}

		virtual void destroy() {
			destroy(true);
		}

		Amara::Entity* setPosition(float gx, float gy) {
			x = gx;
			y = gy;
			return this;
		}
		Amara::Entity* setPosition(FloatVector2 v) {
			return setPosition(v.x, v.y);
		}
		Amara::Entity* setPosition(Amara::Entity* entity) {
			return setPosition(entity->x, entity->y);
		}

		Amara::Entity* setVisible(bool val) {
			isVisible = val;
			return this;
		}

		Amara::Entity* setAlpha(float nAlpha) {
			alpha  = nAlpha;
			if (alpha < 0) alpha = 0;
			if (alpha > 1) alpha = 1;
			return this;
		}

		Amara::Entity* setScale(float gx, float gy) {
			scaleX = gx;
			scaleY = gy;
			return this;
		}
		Amara::Entity* setScale(float g) {
			setScale(g, g);
			return this;
		}

		Amara::Entity* changeScale(float gx, float gy) {
			scaleX += gx;
			scaleY += gy;
			return this;
		}
		Amara::Entity* changeScale(float gi) {
			return changeScale(gi, gi);
		}
		
		float setRadianAngle(float gAngle) {
			angle = toDegrees(gAngle);
			return angle;
		}

		Amara::Entity* setScrollFactor(float gx, float gy) {
			scrollFactorX = gx;
			scrollFactorY = gy;
			return this;
		}
		Amara::Entity* setScrollFactor(float gi) {
			return setScrollFactor(gi, gi);
		}

		Amara::Entity* setZoomFactor(float gx, float gy) {
			zoomFactorX = gx;
			zoomFactorY = gy;
			return this;
		}
		Amara::Entity* setZoomFactor(float gi) {
			setZoomFactor(gi, gi);
			return this;
		}

		Amara::Entity* fixToCamera() {
			setScrollFactor(0);
			setZoomFactor(0);
			return this;
		}

		void resetPassOnProperties() {
			properties->zoomX = 1;
			properties->zoomY = 1;
			properties->zoomFactorX = 1;
			properties->zoomFactorY = 1;
			properties->angle = 0;
			properties->offsetX = 0;
			properties->offsetY = 0;
			properties->scrollX = 0;
			properties->scrollY = 0;
			properties->alpha = 1;
		}

		Amara::Entity* bringToFront() {
			if (parent) {
				for (Amara::Entity* entity: parent->children) {
					if (entity != this && entity->parent == parent && !entity->isDestroyed && depth <= entity->depth) {
						depth = entity->depth + 0.1;
					}
				}
				for (Amara::Entity* entity: parent->entityBuffer) {
					if (entity != this && entity->parent == parent && !entity->isDestroyed && depth <= entity->depth) {
						depth = entity->depth + 0.1;
					}
				}
				parent->sortChildrenOnce = true;
			}
			return this;
		}

		Amara::Entity* sendToBack() {
			for (Amara::Entity* entity: parent->children) {
				if (entity != this && !entity->isDestroyed && depth >= entity->depth) {
					depth = entity->depth - 1;
				}
				parent->sortChildrenOnce = true;
			}
			return this;
		}

		Amara::Entity* attachTo(Amara::Entity* entity) {
			attachedTo = entity;
			if (entity == nullptr) return this;
			x = attachedTo->x + attachmentOffsetX;
			y = attachedTo->y + attachmentOffsetY;
			return this;
		}
		Amara::Entity* attachTo(Amara::Entity* entity, float gx, float gy) {
			setAttachmentOffset(gx, gy);
			return attachTo(entity);
		}
		Amara::Entity* dettach() {
			attachedTo = nullptr;
			return this;
		}
		Amara::Entity* setAttachmentOffset(float gx, float gy) {
			attachmentOffsetX = gx;
			attachmentOffsetY = gy;
			return attachTo(attachedTo);
		}
		Amara::Entity* setAttachementOffset(float gi) {
			return setAttachmentOffset(gi, gi);
		}

		void setLoader(Amara::Loader* gLoader, bool recursive) {
			load = gLoader;
			if (recursive) {
				for (Amara::Entity* entity: children) {
					if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
						continue;
					}
					entity->setLoader(gLoader);
				}
			}
		}

		void setLoader(Amara::Loader* gLoader) {
			setLoader(gLoader, true);
		}

		virtual void reloadAssets() {}
		void reloadChildrenAssets(bool recursive) {
			Amara::Entity* entity;
			for (auto it = children.begin(); it != children.end(); it++) {
				entity = *it;
				entity->reloadAssets();
				if (recursive) entity->reloadChildrenAssets(recursive);
			}
		}
		void reloadChildrenAssets() {
			reloadChildrenAssets(false);
		}

		virtual void preload() {}
		virtual void create() {}
		virtual void update() {}

		virtual ~Entity() {
			if (physics != nullptr && physics->deleteWithParent) {
				physics->destroy();
				physics = nullptr;
			}
		}
	};
	bool Amara::Entity::debuggingDefault = false;
}