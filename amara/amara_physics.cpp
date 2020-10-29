#pragma once 

#include "amara.h"

namespace Amara {
    class PhysicsBody: public Amara::PhysicsBase {
    public:
        int checkDivision = 5;
        int correctionDivision = 1;

        bool hasCollided() {
            for (Entity* entity: collisionTargets) {
                if (collidesWith(entity)) {
                    return true;
                }
            }
            return false;
        }

        void run() {
            velocityX += accelerationX;
            velocityY += accelerationY;
            float recX = parent->x;
            float recY = parent->y;
            float targetX = recX + velocityX;
            float targetY = recY + velocityY;

            if (!hasCollided()) {
                parent->x = targetX;
                updateProperties();
                if (hasCollided()) {
                    while (hasCollided()) {
                        int xDir = velocityX/abs(velocityX);
                        parent->x -= xDir;
                        updateProperties();
                    }
                    velocityX = 0;
                }

                parent->y = targetY;
                updateProperties();
                if (hasCollided()) {
                    while (hasCollided()) {
                        int yDir = velocityY/abs(velocityY);
                        parent->y -= yDir;
                        updateProperties();
                    }
                    velocityY = 0;
                }

                velocityX = velocityX * frictionX;
                velocityY = velocityY * frictionY;
            }
		}
    };

    class PhysicsRectangle: public Amara::PhysicsBody {
    public:
        float width = 0;
        float height = 0;

        PhysicsRectangle() {
            shape = PHYSICS_RECTANGLE;
            properties.rect = { 0, 0, 0, 0 };
        }
        PhysicsRectangle(float gWidth, float gHeight): PhysicsRectangle() {
            width = gWidth;
            height = gHeight;
        }
        PhysicsRectangle(float gx, float gy, float gWidth, float gHeight): PhysicsRectangle(gWidth, gHeight) {
            x = gx;
            y = gy;
        }

        void updateProperties() {
            properties.rect.x = parent->x + x;
            properties.rect.y = parent->y + y;
            properties.rect.width = width;
            properties.rect.height = height;
        }

        bool collidesWith(Entity* other) {
            PhysicsBase* body = other->physics;
            switch (body->shape) {
                case PHYSICS_RECTANGLE:
                    other->physics->updateProperties();
                    return Amara::overlapping(&properties.rect, &body->properties.rect);
                    break;
                case PHYSICS_CIRCLE:
                    other->physics->updateProperties();
                    return Amara::overlapping(&properties.rect, &body->properties.circle);
                    break;
            }

            return other->physics->collidesWith(parent);
        }
    };

    class PhysicsCircle: public Amara::PhysicsBody {
    public:
        float radius = 0;

        PhysicsCircle() {
            properties.circle = { 0, 0, 0 };
            shape = PHYSICS_CIRCLE;
        }
        PhysicsCircle(float gRadius): PhysicsCircle() {
            radius = gRadius; 
        }
        PhysicsCircle(float gx, float gy, float gRadius): PhysicsCircle(gRadius)  {
            x = gx;
            y = gy;
        }

        bool collidesWith(Entity* other) {
            PhysicsBase* body = other->physics;
            switch (body->shape) {
                case PHYSICS_RECTANGLE:
                    other->physics->updateProperties();
                    return Amara::overlapping(&properties.circle, &body->properties.rect);
                    break;
                case PHYSICS_CIRCLE:
                    other->physics->updateProperties();
                    return Amara::overlapping(&properties.circle, &body->properties.circle);
                    break;
            }

            return other->physics->collidesWith(parent);
        }
        
        void updateProperties() {
            properties.circle.x = parent->x + x;
            properties.circle.y = parent->y + y;
            properties.circle.radius = radius;
        }


    };

    class PhysicsTilemapLayer: public Amara::PhysicsBody {
    public:
        float offsetX = 0;
        float offsetY = 0;

        int checkPadding = 1;

        PhysicsTilemapLayer() {
            shape = PHYSICS_TILEMAP_LAYER;
        }

        void create() {
            properties.tilemapLayer = (Amara::TilemapLayer*)parent;
        }

        bool collidesWith(Entity* other) {
            PhysicsBase* body = other->physics;
            TilemapLayer* tilemapLayer = properties.tilemapLayer;

            float sx, sy, ex, ey, tx, ty;
            other->physics->updateProperties();
            switch (body->shape) {
                case PHYSICS_RECTANGLE:
                    sx = body->properties.rect.x + body->properties.rect.width/2.0;
                    sy = body->properties.rect.y + body->properties.rect.height/2.0;
                    break;
                case PHYSICS_CIRCLE:
                    sx = body->properties.circle.x;
                    sy = body->properties.circle.y;
                    break;
            }

            float px = 0;
            float py = 0;
            if (tilemapLayer->tilemapEntity) {
                px = tilemapLayer->tilemapEntity->x;
                py = tilemapLayer->tilemapEntity->y;
            }

            Tile& centerTile = tilemapLayer->getTileAtXY(sx, sy);
            ex = centerTile.x + checkPadding;
            ey = centerTile.y + checkPadding;
            sx = centerTile.x - checkPadding;
            sy = centerTile.y - checkPadding;
            for (int i = sx; i <= ex; i++) {
                for (int j = sy; j <= ey; j++) {
                    Tile& tile = tilemapLayer->getTileAt(i, j);
                    if (tile.id == -1) continue;
                    tx = tile.x * tilemapLayer->tileWidth + px;
                    ty = tile.y * tilemapLayer->tileHeight + py;

                    properties.rect = { tilemapLayer->x + tx, tilemapLayer->y + ty, tilemapLayer->tileWidth, tilemapLayer->tileHeight };
                    switch (body->shape) {
                        case PHYSICS_RECTANGLE:
                            if (Amara::overlapping(&properties.rect, &body->properties.rect)) {
                                return true;
                            }
                            break;
                        case PHYSICS_CIRCLE:
                            if (Amara::overlapping(&properties.rect, &body->properties.circle)) {
                                return true;
                            }
                            break;
                    }
                }
            }


            return false;
        }
    };
}