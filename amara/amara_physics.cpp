namespace Amara {
    void Amara::PhysicsBase::checkActiveCollisionTargets() {
        Amara::PhysicsBase* other;
        for (auto it = collisionTargets.begin(); it != collisionTargets.end();) {
            other = *it;
            if (other->isDestroyed || (other->parent != nullptr && other->parent->isDestroyed)) {
                it = collisionTargets.erase(it);
                continue;
            }
            ++it;
        }
    }

    class PhysicsBody: public Amara::PhysicsBase {
    public:
        using Amara::PhysicsBase::addCollisionTarget;
        void addCollisionTarget(Amara::Entity* other) {
            if (other->physics) addCollisionTarget(other->physics);
        }

        using Amara::PhysicsBase::removeCollisionTarget;
        Amara::PhysicsBase* removeCollisionTarget(Amara::Entity* other) {
            return removeCollisionTarget(other->physics);
        }

        using Amara::PhysicsBase::collidesWith;
        bool collidesWith(Amara::Entity* other) {
            return collidesWith(other->physics);
        }

        using Amara::PhysicsBase::willCollideWith;
        bool willCollideWith(Amara::Entity* other) {
            return willCollideWith(other->physics);
        }
        bool willCollideWith(Amara::PhysicsBase* other) {
            bool collided = false;

            float recVelX = velocityX + accelerationX;
            float recVelY = velocityY + accelerationY;

            float recX = (parent) ? parent->x : x;
            float recY = (parent) ? parent->y : y;

            if (parent) {
                parent->x += recVelX;
                parent->y += recVelY;
            }
            else {
                x += recVelX;
                y += recVelY;
            }

            updateProperties();
            collided = collidesWith(other);

            if (parent) {
                parent->x = recX;
                parent->y = recY;
            }
            else {
                x = recX;
                y = recY;
            }

            return collided;
        }

        bool willCollide() {
            Amara::PhysicsBase* body;
            for (auto it = collisionTargets.begin(); it != collisionTargets.end();) {
                body = *it;
                if (body->isDestroyed || (body->parent != nullptr && body->parent->isDestroyed)) {
                    ++it;
                    continue;
                }
				else if (!body->isActive) {
                    ++it;
                    continue;
                }
                else if (willCollideWith(body)) {
                    return true;
                }
                ++it;
            }
            return false;
        }

        using Amara::PhysicsBase::hasCollided;
        bool hasCollided(bool pushingX, bool pushingY) {
			if (!isColliding) return false;
            bool col = false;
            Amara::PhysicsBase* body;
            for (auto it = collisionTargets.begin(); it != collisionTargets.end();) {
                body = *it;
                if (body->isDestroyed || (body->parent != nullptr && body->parent->isDestroyed)) {
                    ++it;
                    continue;
                }
				else if (!body->isActive) { ++it; continue; }
				else if (!body->isWall) { ++it; continue; }
                else if (collidesWith(body)) {
                    bumped = body;
                    if (body->isPushable) {
                        if (pushingX) body->velocityX += velocityX * body->pushFrictionX;
                        if (pushingY) body->velocityY += velocityY * body->pushFrictionY;
                    }
                    if (pushingX || pushingY) {
                        if (body->isPushable) isPushing = true;
                        col = true;
                        ++it;
                        continue;
                    }
                    return true;
                }
                ++it;
            }
            return col;
        }

        virtual bool outOfBounds() { return false; }

        void run() {
            bumped = nullptr;
            bumpDirections = 0;
            isPushing = false;

            if (isPaused || !isActive) {
                updateProperties();
                return;
            }

            velocityX += accelerationX;
            velocityY += accelerationY;
            float recX = (parent) ? parent->x : x;
            float recY = (parent) ? parent->y : y;
			double correctionTotal, correctionChange;

            if (!hasCollided() || outOfBounds()) {
				if (abs(velocityX) > 0) {
					correctionTotal = 0;
					correctionChange = correctionRate * (velocityX/abs(velocityX));
					while (abs(correctionTotal) < abs(velocityX)) {
						correctionTotal += correctionChange;
						if (abs(correctionTotal) > abs(velocityX)) correctionTotal = velocityX;
						if (parent) parent->x = recX + correctionTotal;
						else x = recX + correctionTotal;
						updateProperties();
						if (hasCollided(true, false) || outOfBounds()) {
							correctionTotal -= correctionChange;
							if (correctionChange > 0 && correctionTotal < 0) correctionTotal = 0;
							if (correctionChange < 0 && correctionTotal > 0) correctionTotal = 0;

							if (correctionTotal > 0) bumpDirections += Right;
							else if (correctionTotal < 0) bumpDirections += Left;
							else bumpDirections += Left + Right;

							velocityX = -velocityX * bounceX;
							break;
						}
					}
					if (parent) parent->x = recX + correctionTotal;
					else x = recX + correctionTotal;
					updateProperties();
				}

				if (abs(velocityY) > 0) {
					correctionTotal = 0;
					correctionChange = correctionRate * (velocityY/abs(velocityY));
					while (abs(correctionTotal) < abs(velocityY)) {
						correctionTotal += correctionChange;
						if (abs(correctionTotal) > abs(velocityY)) correctionTotal = velocityY;
						if (parent) parent->y = recY + correctionTotal;
						else y = recY + correctionTotal;
						updateProperties();
						if (hasCollided(false, true) || outOfBounds()) {
							correctionTotal -= correctionChange;
							if (correctionChange > 0 && correctionTotal < 0) correctionTotal = 0;
							if (correctionChange < 0 && correctionTotal > 0) correctionTotal = 0;

							if (correctionTotal > 0) bumpDirections += Down;
							else if (correctionTotal < 0) bumpDirections += Up;
							else bumpDirections += Down + Up;

							velocityY = -velocityY * bounceY;
							break;
						}
					}
					if (parent) parent->y = recY + correctionTotal;
					else y = recY + correctionTotal;
					updateProperties();
				}

                if (bumpDirections & Up) {
                    if (bumpDirections & Left) bumpDirections += UpLeft;
                    if (bumpDirections & Right) bumpDirections += UpRight;
                }
                if (bumpDirections & Down) {
                    if (bumpDirections & Left) bumpDirections += DownLeft;
                    if (bumpDirections & Right) bumpDirections += DownRight;
                }

                velocityX = velocityX * frictionX;
                velocityY = velocityY * frictionY;
            }
		}

        void destroy() {
            Amara::PhysicsBase::destroy();
            if (parent) {
                parent->physics = nullptr;
                parent = nullptr;
            }
        }
    };

    class PhysicsRectangle: public Amara::PhysicsBody {
    public:
        float width = 0;
        float height = 0;

        PhysicsRectangle() {
            properties.shape = PHYSICS_RECTANGLE;
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
            properties.rect.x = ((parent) ? parent->x : 0) + x;
            properties.rect.y = ((parent) ? parent->y : 0) + y;
            properties.rect.width = width;
            properties.rect.height = height;
        }

        bool collidesWith(Amara::PhysicsBase* body) {
            if (!body->isActive) return false;
            switch (body->properties.shape) {
                case PHYSICS_RECTANGLE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.rect, &body->properties.rect);
                    break;
                case PHYSICS_CIRCLE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.rect, &body->properties.circle);
                    break;
                case PHYSICS_LINE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.rect, &body->properties.line);
                    break;
            }

            return body->collidesWith(this);
        }

        bool outOfBounds() {
            if (lockedToBounds) {
                if (properties.rect.x < boundX) return true;
                if (properties.rect.y < boundY) return true;
                if (properties.rect.x + properties.rect.width > boundX + boundW) return true;
                if (properties.rect.y + properties.rect.height > boundY + boundH) return true; 
            }
            return false;
        }
    };

    class PhysicsCircle: public Amara::PhysicsBody {
    public:
        float radius = 0;

        PhysicsCircle() {
            properties.shape = PHYSICS_CIRCLE;
            properties.circle = { 0, 0, 0 };
        }
        PhysicsCircle(float gRadius): PhysicsCircle() {
            radius = gRadius; 
        }
        PhysicsCircle(float gx, float gy, float gRadius): PhysicsCircle(gRadius)  {
            x = gx;
            y = gy;
        }

        void updateProperties() {
            properties.circle.x = ((parent) ? parent->x : 0) + x;
            properties.circle.y = ((parent) ? parent->y : 0) + y;
            properties.circle.radius = radius;
        }

        bool collidesWith(Amara::PhysicsBase* body) {
            if (!body->isActive) return false;
            switch (body->properties.shape) {
                case PHYSICS_RECTANGLE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.circle, &body->properties.rect);
                    break;
                case PHYSICS_CIRCLE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.circle, &body->properties.circle);
                    break;
                case PHYSICS_LINE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.circle, &body->properties.line);
                    break;
            }

            return body->collidesWith(this);
        }

        bool outOfBounds() {
            if (lockedToBounds) {
                if (properties.circle.x - properties.circle.radius < boundX) return true;
                if (properties.circle.y - properties.circle.radius < boundY) return true;
                if (properties.circle.x + properties.circle.radius > boundX + boundW) return true;
                if (properties.circle.y + properties.circle.radius > boundY + boundH) return true;
            }
            return false;
        }
    };

    class PhysicsLine: public Amara::PhysicsBody {
    public:
        FloatVector2 p1 = {0, 0};
        FloatVector2 p2 = {0, 0};

        PhysicsLine() {
            properties.line = {{0, 0}, {0, 0}};
            properties.shape = PHYSICS_LINE;
        }
        PhysicsLine(float p1x, float p1y, float p2x, float p2y): PhysicsLine() {
            float ex = ((parent) ? parent->x : 0);
            float ey = ((parent) ? parent->y : 0);
            p1 = {p1x + ex, p1y + ey};
            p2 = {p2x + ex, p2y + ey};
        }

        void updateProperties() {
            properties.line.p1 = {p1.x + x, p1.y + y};
            properties.line.p2 = {p2.x + x, p2.y + y};
        }

        bool collidesWith(Amara::PhysicsBody* body) {
            if (!body->isActive) return false;
            switch (body->properties.shape) {
                case PHYSICS_RECTANGLE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.line, &body->properties.rect);
                    break;
                case PHYSICS_CIRCLE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.line, &body->properties.circle);
                    break;
                case PHYSICS_LINE:
                    body->updateProperties();
                    return Amara::overlapping(&properties.line, &body->properties.line);
                    break;
            }

            return body->collidesWith(this);
        }

        bool outOfBounds() {
            if (lockedToBounds) {
                if (properties.line.p1.x < boundX) return true;
                if (properties.line.p1.x > boundX + boundW) return true;
                if (properties.line.p1.y < boundY) return true;
                if (properties.line.p1.y > boundY + boundH) return true;

                if (properties.line.p2.x < boundX) return true;
                if (properties.line.p2.x > boundX + boundW) return true;
                if (properties.line.p2.y < boundY) return true;
                if (properties.line.p2.y > boundY + boundH) return true;
            }
            return true;
        }
    };

    class PhysicsTilemapLayer: public Amara::PhysicsBody {
    public:
        float defaultProgressRate = 0.1;
        int checkPadding = 1;

        PhysicsProperties tileProperties;

        PhysicsTilemapLayer() {
            properties.shape = PHYSICS_TILEMAP_LAYER;
        }
        PhysicsTilemapLayer(float gx, float gy): PhysicsTilemapLayer() {
            x = gx;
            y = gy;
        }

        void create() {
            properties.tilemapLayer = (Amara::TilemapLayer*)parent;
        }

        bool lineCollision(Amara::PhysicsBase* body, float progRate) {
            Amara::TilemapLayer* tilemapLayer = properties.tilemapLayer;

            float sx, sy, tx, ty;
            float px = x;
            float py = y;
            if (tilemapLayer->tilemapEntity) {
                px += tilemapLayer->tilemapEntity->x;
                py += tilemapLayer->tilemapEntity->y;
            }

            for (float prog = 0; prog <= 1; prog += progRate) {
                sx = body->properties.line.p1.x + (body->properties.line.p2.x - body->properties.line.p1.x)*prog;
                sy = body->properties.line.p1.y + (body->properties.line.p2.y - body->properties.line.p1.y)*prog;

                Tile& tile = tilemapLayer->getTileAtXY(sx, sy);
                if (tile.id == -1) continue;
                tx = tile.x * tilemapLayer->tileWidth + px;
                ty = tile.y * tilemapLayer->tileHeight + py;

                if (tilemapLayer->tileHitboxes.find(tile.id) != tilemapLayer->tileHitboxes.end()) {
                    tileProperties = tilemapLayer->tileHitboxes[tile.id];
                }
                else {
                    tileProperties.setRect(tilemapLayer->x + tx, tilemapLayer->y + ty, tilemapLayer->tileWidth, tilemapLayer->tileHeight);
                }

                switch (tileProperties.shape) {
                    case PHYSICS_RECTANGLE:
                        if (collideUsingTileRect(body)) return true;
                        break;
                    case PHYSICS_CIRCLE:
                        if (collideUsingTileCircle(body)) return true;
                        break;
                    case PHYSICS_LINE:
                        if (collideUsingTileLine(body)) return true;
                        break;
                }
            }
            return false;
        }
        bool lineCollision(Amara::PhysicsBase* body) {
            return lineCollision(body, defaultProgressRate);
        }

        bool collidesWith(Amara::PhysicsBase* body) {
            if (!body->isActive) return false;
            Amara::TilemapLayer* tilemapLayer = properties.tilemapLayer;

            float sx, sy, ex, ey, tx, ty;
            body->updateProperties();
            switch (body->properties.shape) {
                case PHYSICS_RECTANGLE:
                    sx = body->properties.rect.x + body->properties.rect.width/2.0;
                    sy = body->properties.rect.y + body->properties.rect.height/2.0;
                    break;
                case PHYSICS_CIRCLE:
                    sx = body->properties.circle.x;
                    sy = body->properties.circle.y;
                    break;
                case PHYSICS_LINE:
                    return lineCollision(body);
                    break;
            }

            float px = x;
            float py = y;
            if (tilemapLayer->tilemapEntity) {
                px += tilemapLayer->tilemapEntity->x;
                py += tilemapLayer->tilemapEntity->y;
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

                    if (tilemapLayer->tileHitboxes.find(tile.id) != tilemapLayer->tileHitboxes.end()) {
                        tileProperties = tilemapLayer->tileHitboxes[tile.id];
                    }
                    else {
                        tileProperties.setRect(tilemapLayer->x + tx, tilemapLayer->y + ty, tilemapLayer->tileWidth, tilemapLayer->tileHeight);
                    }
                    
                    switch (tileProperties.shape) {
                        case PHYSICS_RECTANGLE:
                            if (collideUsingTileRect(body)) return true;
                            break;
                        case PHYSICS_CIRCLE:
                            if (collideUsingTileCircle(body)) return true;
                            break;
                        case PHYSICS_LINE:
                            if (collideUsingTileLine(body)) return true;
                            break;
                    }
                }
            }

            return false;
        }

        bool collideUsingTileRect(Amara::PhysicsBase* body) {
            switch (body->properties.shape) {
                case PHYSICS_RECTANGLE:
                    if (Amara::overlapping(&tileProperties.rect, &body->properties.rect)) {
                        return true;
                    }
                    break;
                case PHYSICS_CIRCLE:
                    if (Amara::overlapping(&tileProperties.rect, &body->properties.circle)) {
                        return true;
                    }
                    break;
                case PHYSICS_LINE:
                    if (Amara::overlapping(&tileProperties.rect, &body->properties.line)) {
                        return true;
                    }
                    break;
            }
            return false;
        }

        bool collideUsingTileCircle(Amara::PhysicsBase* body) {
            switch (body->properties.shape) {
                case PHYSICS_RECTANGLE:
                    if (Amara::overlapping(&tileProperties.circle, &body->properties.rect)) {
                        return true;
                    }
                    break;
                case PHYSICS_CIRCLE:
                    if (Amara::overlapping(&tileProperties.circle, &body->properties.circle)) {
                        return true;
                    }
                    break;
                case PHYSICS_LINE:
                    if (Amara::overlapping(&tileProperties.circle, &body->properties.line)) {
                        return true;
                    }
                    break;
            }
            return false;
        }

        bool collideUsingTileLine(Amara::PhysicsBase* body) {
            switch (body->properties.shape) {
                case PHYSICS_RECTANGLE:
                    if (Amara::overlapping(&tileProperties.line, &body->properties.rect)) {
                        return true;
                    }
                    break;
                case PHYSICS_CIRCLE:
                    if (Amara::overlapping(&tileProperties.line, &body->properties.circle)) {
                        return true;
                    }
                    break;
                case PHYSICS_LINE:
                    if (Amara::overlapping(&tileProperties.line, &body->properties.line)) {
                        return true;
                    }
                    break;
            }
            return false;
        }
    };

    class PhysicsCollisionGroup: public PhysicsBody {
    public:
        std::vector<Amara::PhysicsBase*>members;

        PhysicsCollisionGroup() {
            members.clear();
        }

        Amara::PhysicsBase* add(Amara::Entity* gEntity) {
            if (gEntity->physics == nullptr) return nullptr;
            members.push_back(gEntity->physics);
            return gEntity->physics;
        }
        Amara::PhysicsBase* add(Amara::PhysicsBase* other) {
            if (other) members.push_back(other);
            return other;
        }
        Amara::PhysicsBase* remove(Amara::Entity* gEntity) {
            return remove(gEntity->physics);
        }
        Amara::PhysicsBase* remove(Amara::PhysicsBase* gBody) {
            PhysicsBase* member;
            for (auto it = members.begin(); it != members.end();) {
                member = *it;
                if (member == gBody) {
                    it = members.erase(it);
                    return gBody;
                }
                ++it;
            }
            return nullptr;
        }
        void clear() {
            members.clear();
        }

        bool collidesWith(Amara::PhysicsBase* other) {
            for (auto it = members.begin(); it != members.end();) {
                Amara::PhysicsBase* body = *it;
                if (body->isDestroyed || (body->parent != nullptr && body->parent->isDestroyed)) {
                    it = members.erase(it);
                    continue;
                }
                if (body == other) { ++it; continue; }
                if (!body->isActive) { ++it; continue; }
                if (body->collidesWith(other)) {
                    return true;
                }
                ++it;
            }
            return false;
        }

        void run() {}
    };
}