namespace Amara {
    int findPath(void* data);

    enum PathTileState {
        PATHTILE_NA,
        PATHTILE_OPEN,
        PATHTILE_CLOSED
    };

    class PathTile {
        public:
            int id = -1;
            int parentId = -1;
            int x = 0;
            int y = 0;
            int gcost = 0;
            int hcost = 0;
            int fcost = 0;
            Amara::Direction direction = NoDir;
            PathTileState state = PATHTILE_NA;
            bool empty = false;
    };

    class PathFindingTask {
        public:
            Amara::WallFinder* wallFinder;
            int startX = 0;
            int startY = 0;
            int targetX = 0;
            int targetY = 0;

            int startId = -1;
            int targetId = -1;

            std::deque<Amara::PathTile> path;
            std::vector<Amara::PathTile> allTiles;
            Amara::PathTile emptyTile;

            SDL_Thread* thread = nullptr;
            bool findingPath = false;
            bool foundPath = false;

            bool allowDiagonals = false;

            int width = 0;
            int height = 0;

            PathFindingTask() {}
            PathFindingTask(Amara::WallFinder* gWallFinder) {
                configure(gWallFinder);
            }

            void configure(Amara::WallFinder* gWallFinder) {
                wallFinder = gWallFinder;
                width = gWallFinder->getMapWidth();
                height = gWallFinder->getMapHeight();
                emptyTile.empty = true;
            }

            void calculateCosts(PathTile& child, PathTile& parent) {
                child.gcost = parent.gcost + distanceBetween(child, parent);
                child.hcost = distanceBetween(child, getTile(targetId));
                child.fcost = child.gcost + child.hcost;
            }

            void calculateCosts(PathTile& tile) {
				tile.gcost = distanceBetween(tile, getTile(startId));
				tile.hcost = distanceBetween(tile, getTile(targetId));
				tile.fcost = tile.gcost + tile.hcost;
			}

            void calculateCosts(int id) {
                calculateCosts(getTile(id));
            }

            void calculateCosts(int child, int parent) {
                calculateCosts(getTile(child), getTile(parent));
            }

            int distanceBetween(PathTile& fromTile, PathTile& toTile) {
                return (Amara::distanceBetween(fromTile.x, fromTile.y, toTile.x, toTile.y) * 10.0);
            }

            int distanceBetween(int from, int to) {
                return distanceBetween(getTile(from), getTile(to));
            }

            int lowestFCost(std::list<int> list) {
                int smallest = -1;
                int smallestFCost = 0;
                for (int id: list) {
                    Amara::PathTile& tile = getTile(id);
                    if (smallest == -1 || tile.fcost < smallestFCost) {
                        smallest = id;
                        smallestFCost = tile.fcost;
                    }
                }
                return smallest;
            }

            bool listContains(std::list<int> list, int val) {
                return (std::find(list.begin(), list.end(), val) != list.end());
            }

            Amara::PathTile& getTileAt(int gx, int gy) {
                return allTiles[gy*width + gx];
            }
            Amara::PathTile& getTile(int id) {
                return allTiles[id];
            }

            Amara::PathTile& getPathTileAt(int gx, int gy) {
                if (path.size() > 0) {
                    Amara::PathTile tile;
                    while (path.size() > 0) {
                        tile = dequeue();
                        if (tile.x == gx && tile.y == gy) {
                            return tile;
                        }
                    }
                }
                foundPath = false;

                return emptyTile;
            }

            bool isWall(int gx, int gy) {
                return wallFinder->isWall(gx, gy);
            }

            Amara::PathFindingTask* from(int gx, int gy) {
                startX = gx;
                startY = gy;
                return this;
            }
            Amara::PathFindingTask* to(int gx, int gy) {
                targetX = gx;
                targetY = gy;
                return this;
            }

            Amara::PathTile dequeue() {
                if (path.size() > 0) {
                    Amara::PathTile tile = path.front();
                    path.pop_front();
                    return tile;
                }
                foundPath = false;
                return emptyTile;
            }

            Amara::PathFindingTask* start() {
                emptyTile.empty = true;
                if (findingPath || thread != nullptr) {
                    return this;
                }

                path.clear();
                findingPath = true;
                foundPath = false;

                Amara::PathTile egTile;
                allTiles.resize(width*height, egTile);
                
                int size = allTiles.size();
                for (int i = 0; i < size; i++) {
                    Amara::PathTile& tile = allTiles[i];
                    tile.id = i;
                    tile.x = i % width;
                    tile.y = floor(((float)i) / (float)width);
                    tile.state = PATHTILE_NA;
                    tile.parentId = -1;
                    tile.direction = NoDir;

                    if (tile.x == startX && tile.y == startY) {
                        startId = tile.id;
                    }
                    if (tile.x == targetX && tile.y == targetY) {
                        targetId = tile.id;
                    }
                }

                SDL_Thread* thread = SDL_CreateThread(findPath, NULL, this);
                SDL_DetachThread(thread);
                return this;
            }

            Amara::PathFindingTask* run(int sx, int sy, int ex, int ey) {
                return from(sx, sy)->to(ex, ey)->start();
            }
    };

    int findPath(void* data) {
        Amara::PathFindingTask* task = (Amara::PathFindingTask*)data;
        std::list<int> openList;
        std::list<int> closedList;

        task->wallFinder->locked = true;

        int startId = task->startId;
        int targetId = task->targetId;

        int current = -1;

        bool findingPath = true;
        bool foundPath = false;
        task->calculateCosts(startId);
        openList.push_back(startId);

        while (findingPath) {
            if (openList.empty()) {
                findingPath = false;
                foundPath = false;
                break;
            }

            current = task->lowestFCost(openList);
            openList.remove(current);
            closedList.push_back(current);
            if (current == targetId) {
                findingPath = false;
                foundPath = true;
                break;
            }

            Amara::PathTile& curTile = task->getTile(current);

            std::vector<int> neighbours;
            std::vector<Amara::Direction> dirs;
            if (task->allowDiagonals) {
                dirs = Amara::DirectionsInOrder;
            }
            else {
                dirs = Amara::CardinalDirections;
            }
            int ox, oy, nId;
            for (Amara::Direction dir: dirs) {
                ox = Amara::getOffsetX(dir);
                oy = Amara::getOffsetY(dir);
                nId = current + ox + (oy * task->width);
                Amara::PathTile& tile = task->getTile(nId);
                if (nId < 0 || nId >= task->width*task->height || task->listContains(closedList, nId) || task->isWall(tile.x, tile.y)) {
                    continue;
                }
                
                if (!task->listContains(openList, nId)) {
                    openList.push_back(nId);
                    tile.parentId = current;
                    tile.direction = dir;
                    task->calculateCosts(nId, current);
                }
                else {
                    if (curTile.gcost + task->distanceBetween(current, nId) < tile.gcost) {
                        tile.parentId = current;
                        tile.direction = dir;
                        task->calculateCosts(nId, current);
                    }
                }
            }
        }
        
        if (foundPath) {
            int current = targetId;
            while (current != -1) {
                Amara::PathTile& curTile = task->getTile(current);
                task->path.push_front(curTile);
                current = curTile.parentId;
            }
        }
        else {
            SDL_Log("Path not found: from (%d,%d) to (%d,%d)", task->startX, task->startY, task->targetX, task->targetY);
        }
        
        task->findingPath = false;
        task->thread = nullptr;
        task->foundPath = foundPath;
        task->wallFinder->locked = false;
        return 0;
    }
}