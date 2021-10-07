#pragma once
#ifndef AMARA_ENTITY
#define AMARA_ENTITY

#include "amara.h"

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

	struct sortEntities {
		inline bool operator() (Amara::SortedEntity* entity1, Amara::SortedEntity* entity2) {
			if (entity1 == nullptr) return false;
			if (entity2 == nullptr) return false;
			return (entity1->depth < entity2->depth);
		}
	};

	class Entity : public Amara::SortedEntity, public Amara::Interactable {
		public:
			Amara::GameProperties* properties = nullptr;
			Amara::Game*  game = nullptr;
			Amara::Scene* scene = nullptr;
			Amara::Entity* parent = nullptr;

			Amara::Entity* attachedTo = nullptr;
			float attachmentOffsetX = 0;
			float attachmentOffsetY = 0;

			Amara::InputManager* input = nullptr;
			Amara::ControlScheme* controls = nullptr;
			Amara::AudioGroup* audio = nullptr;
			Amara::AssetManager* assets = nullptr;
			Amara::Loader* load = nullptr;
			Amara::MessageQueue* messages = nullptr;

			std::vector<Amara::Entity*> entities;

			Amara::PhysicsBase* physics = nullptr;
			bool pushedMessages = false;

			nlohmann::json data;

			std::string id;
			std::string entityType;

			float x = 0;
			float y = 0;
			float z = 0;

			float scaleX = 1;
			float scaleY = 1;

			float cameraOffsetX = 0;
			float cameraOffsetY = 0;

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
				messages = properties->messages;

				isActive = true;
				entityType = "entity";

				init();
				if (!isDestroyed) create();
			}

			virtual void init(Amara::GameProperties* gameProperties, Amara::Scene* givenScene) {
				init(gameProperties, givenScene, nullptr);
			}

			virtual void init() {}

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
				if (config.find("data") != config.end()) {
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
			}

			virtual nlohmann::json toData() {
				nlohmann::json config;
				config["id"] = id;
				config["entityType"] = entityType;
				config["x"] = x;
				config["y"] = y;
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
				config["data"] = data;
				return config;
			}
			
			Amara::Entity* setId(std::string newId) {
				id = newId;
				return this;
			}

			bool hasDataProperty(std::string gKey) {
				if (data.find(gKey) != data.end()) {
					return true;
				} 
				return false;
			}

			bool isDataProperty(std::string gKey) {
				if (hasDataProperty(gKey) && data[gKey].is_boolean() && data[gKey]) {
					return true;
				}
				return false;
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

				stable_sort(entities.begin(), entities.end(), sortEntities());

				Amara::Entity* entity;
				for (auto it = entities.begin(); it != entities.end(); ++it) {
                    entity = *it;

                    if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
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
					properties->alpha = recAlpha;
					entity->draw(vx, vy, vw, vh);
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

			virtual void run() {
				receiveMessages();
				updateMessages();

				Amara::Interactable::run();
				update();
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

				Amara::Entity* entity;
				for (auto it = entities.begin(); it != entities.end(); ++it) {
					entity = *it;
					if (entity == nullptr || entity->isDestroyed || entity->parent != this) {
						entities.erase(it--);
						continue;
					}
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
				if (entity && entity->parent != nullptr) {
					entity->parent->remove(entity);
				}
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
				for (auto it = entities.begin(); it != entities.end(); ++it) {
					child = *it;
					if (child == entity) {
						if (entity->parent == this) entity->parent = nullptr;
						entities.erase(it--);
					}
				}
				return nullptr;
			}

			virtual void removeEntities() {
				entities.clear();
			}

			virtual void addPhysics(Amara::PhysicsBase* gPhysics) {
				physics = gPhysics;
				physics->isActive = true;
				physics->parent = this;
				physics->gameProperties = properties;
				physics->updateProperties();
				physics->create();
			}

			virtual Amara::PhysicsBase* removePhysics() {
				Amara::PhysicsBase* rec = physics;
				if (physics) {
					physics = nullptr;
				}
				return rec;
			}
			virtual Amara::PhysicsBase* destroyPhysics() {
				Amara::PhysicsBase* rec = removePhysics();
				if (rec) {
					rec->destroy();
				}
			}

			virtual void destroyChildren() {
				Amara::Entity* child;
				int numChildren = entities.size();
				for (size_t i = 0; i < numChildren; i++) {
					child = entities.at(i);
					child->destroy();
				}
				entities.clear();
			}

			virtual void destroyEntities(bool recursiveDestroy) {
				for (Amara::Entity* child: entities) {
					if (recursiveDestroy) {
						child->destroy();
					}
					else {
						child->parent = nullptr;
					}
				}
				entities.clear();
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
				}
			}

			virtual void destroy() {
				destroy(true);
			}

			void setVisible(bool val) {
				isVisible = val;
			}

			void setAlpha(float nAlpha) {
				alpha  = nAlpha;
				if (alpha < 0) alpha = 0;
				if (alpha > 1) alpha = 1;
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

			virtual void bringToFront() {
				std::vector<Amara::Entity*>& rSceneEntities = parent->entities;
				for (Amara::Entity* entity: rSceneEntities) {
					if (entity != this && !entity->isDestroyed && depth <= entity->depth) {
						depth = entity->depth + 1;
					}
				}
			}

			virtual void sendToBack() {
				std::vector<Amara::Entity*>& rSceneEntities = parent->entities;
				for (Amara::Entity* entity: rSceneEntities) {
					if (entity != this && depth >= entity->depth) {
						depth = entity->depth - 1;
					}
				}
			}

			virtual void attachTo(Amara::Entity* entity) {
				attachedTo = entity;
				if (entity == nullptr) return;
				x = attachedTo->x + attachmentOffsetX;
				y = attachedTo->y + attachmentOffsetY;
			}
			virtual void attachTo(Amara::Entity* entity, float gx, float gy) {
				setAttachmentOffset(gx, gy);
				attachTo(entity);
			}
			virtual void dettach() {
				attachedTo = nullptr;
			}
			void setAttachmentOffset(float gx, float gy) {
				attachmentOffsetX = gx;
				attachmentOffsetY = gy;
				attachTo(attachedTo);
			}
			void setAttachementOffset(float gi) {
				setAttachmentOffset(gi, gi);
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

			void updateMessages() {
				if (pushedMessages) {
					pushedMessages = false;
					Amara::MessageQueue& messages = *(properties->messages);
					for (auto it = messages.begin(); it != messages.end(); ++it) {
						Message msg = *it;
						if (msg.parent == this) {
							if (msg.skip) {
								msg.skip = false;
								pushedMessages = true;
							}
							else messages.queue.erase(it--);
						}
					}
				}
			}
			Message& broadcastMessage(std::string key, nlohmann::json gData) {
				pushedMessages = true;
				return properties->messages->broadcast(this, key, gData);
			}
			Message& getMessage(std::string key) {
				return properties->messages->get(key);
			}
			virtual void receiveMessages() {}

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
