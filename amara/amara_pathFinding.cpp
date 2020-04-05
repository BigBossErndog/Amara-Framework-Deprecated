#ifndef AMARA_PATHFINDING
#define AMARA_PATHFINDING

#include "amara.h"

namespace Amara {
    class Walker;

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
            Amara::Direction direction;
            PathTileState state = PATHTILE_NA;
    };

    class PathFindingTask {
        public:
            std::vector<Amara::TilemapLayer*> walls;
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

            PathFindingTask(std::vector<Amara::TilemapLayer*> gWalls) {
                walls = gWalls;
                width = walls[0]->width;
                height = walls[0]->height;
            }

            PathFindingTask(Amara::TilemapLayer* wallLayer) {
                walls = {wallLayer};
                width = walls[0]->width;
                height = walls[0]->height;
            }

            PathFindingTask(Amara::Tilemap* tilemap) {
                walls = tilemap->walls;
                width = tilemap->width;
                height = tilemap->height;
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
                int distance = 0;
                PathTile cur;
                cur.x = fromTile.x;
                cur.y = fromTile.y;
                while (cur.x != toTile.x && cur.y != toTile.y) {
                    if (allowDiagonals) {
                        if (cur.x < toTile.x && cur.y < toTile.y) {
                            cur.x += 1;
                            cur.y += 1;
                            distance += 14;
                            continue;
                        }
                        if (cur.x < toTile.x && cur.y > toTile.y) {
                            cur.x += 1;
                            cur.y -= 1;
                            distance += 14;
                            continue;
                        }
                        if (cur.x > toTile.x && cur.y < toTile.y) {
                            cur.x -= 1;
                            cur.y += 1;
                            distance += 14;
                            continue;
                        }
                        if (cur.x > toTile.x && cur.y > toTile.y) {
                            cur.x -= 1;
                            cur.y -= 1;
                            distance += 14;
                            continue;
                        }
                    }
                    if (cur.x < toTile.x) {
                        cur.x += 1;
                        distance += 10;
                        continue;
                    }
                    if (cur.x > toTile.x) {
                        cur.x -= 1;
                        distance += 10;
                        continue;
                    }
                    if (cur.y < toTile.y) {
                        cur.y += 1;
                        distance += 10;
                        continue;
                    }
                    if (cur.y > toTile.y) {
                        cur.y -= 1;
                        distance += 10;
                        continue;
                    }
                }

                return distance;
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

            bool isWall(int gx, int gy) {
                Amara::Tile tile;
                for (Amara::TilemapLayer* layer: walls) {
                    tile = layer->getTileAt(gx, gy);
                    if (tile.id >= 0) {
                        return true;
                    }
                }
                return false;
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

            Amara::PathTile& dequeue() {
                if (path.size() > 0) {
                    Amara::PathTile& tile = path.front();
                    path.pop_front();
                    return tile;
                }
                return emptyTile;
            }

            Amara::PathFindingTask* start() {
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
                return this;
            }
    };

    int findPath(void* data) {
        Amara::PathFindingTask* task = (Amara::PathFindingTask*)data;
        std::list<int> openList;
        std::list<int> closedList;

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
                dirs = Amara::FourDirections;
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
        return 0;
    }
}

#endif