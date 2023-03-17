#pragma once
#ifndef AMARA_GEOMETRY
#define AMARA_GEOMETRY



namespace Amara {
    typedef struct IntVector2 {
        int x = 0;
        int y = 0;
    } IntVector2;

    typedef struct FloatVector2 {
        float x = 0;
        float y = 0;
    } FloatVector2;

    typedef struct IntVector3 {
        int x = 0;
        int y = 0;
        int z = 0;
    } IntVector;

    typedef struct FloatVector3{
        float x = 0;
        float y = 0;
        float z = 0;
    } FloatVector3;

    float distanceBetween(float sx, float sy, float ex, float ey) {
        float xDist = ex-sx;
        float yDist = ey-sy;
        return sqrt(xDist*xDist + yDist*yDist);
    }
    float distanceBetween(IntVector2 s, IntVector2 e) {
        return distanceBetween(s.x, s.y, e.x, e.y);
    }
    float distanceBetween(FloatVector2 s, FloatVector2 e) {
        return distanceBetween(s.x, s.y, e.x, e.y);
    }

    float radiansToDegrees(float rads) {
        return rads*180/M_PI;
    }
    float degreesToRadians(float degrees) {
        return degrees*M_PI/180;
    }

    float angleBetween(FloatVector2 p1, FloatVector2 p2) {
        float angle = -atan2(p2.y-p1.y, p2.x-p1.x) + M_PI/2.0;
        angle = fmod(angle, 2*M_PI);
        while (angle < 0) {
            angle += 2*M_PI;
        }
        return angle;
    }
    float angleBetween(float p1x, float p1y, float p2x, float p2y) {
        float angle = -atan2(p2y-p1y, p2x-p1x) + M_PI/2.0;
        angle = fmod(angle, 2*M_PI);
        while (angle < 0) {
            angle += 2*M_PI;
        }
        return angle;
    }

    typedef struct IntRect {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
    } IntRect;

    typedef struct FloatRect {
        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;
    } FloatRect;

    typedef struct FloatCircle {
        float x = 0;
        float y = 0;
        float radius = 0;
    } FloatCircle;
    
    typedef struct FloatLine {
        FloatVector2 p1 = {0, 0};
        FloatVector2 p2 = {0, 0};
    } FloatLine;

    std::vector<FloatVector2> getPointsAlongLine(FloatLine line, int divisions) {
        std::vector<FloatVector2> points;
        points.clear();
        
        float step;
        for (int i = 0; i <= divisions; i++) {
            step = i/(float)divisions;
            FloatVector2 point = {
                line.p1.x + (line.p2.x - line.p1.x)*step,
                line.p1.y + (line.p2.y - line.p1.y)*step
            };
            points.push_back(point);
        }

        return points;
    }
    std::vector<FloatVector2> getPointsAlongLine(FloatVector2 start, FloatVector2 end, int divisions) {
        return getPointsAlongLine({start, end}, divisions);
    }
    std::vector<FloatVector2> getPointsAlongLine(float x1, float y1, float x2, float y2, float divisions) {
        return getPointsAlongLine({{x1, y1}, {x2, y2}}, divisions);
    }

    std::vector<FloatVector2> travelAlongLine(FloatLine line, float step) {
        std::vector<FloatVector2> points;
        points.clear();

        float angle = angleBetween(line.p1, line.p2);
        float fullDist = distanceBetween(line.p1, line.p2);

        bool nowExit = false;
        float curDist = 0;
        while (true) {
            FloatVector2 point = {
                line.p1.x + sin(angle)*curDist,
                line.p1.y + cos(angle)*curDist
            };
            points.push_back(point);
            curDist += step;
            if (nowExit) {
                break;
            }
            else if (curDist >= fullDist) {
                curDist = fullDist;
                nowExit = true;
            }
        }
        return points;
    }

    bool overlapping(float px, float py, FloatCircle* circle) {
        return (Amara::distanceBetween(px, py, circle->x, circle->y) <= circle->radius);
    }
    bool overlapping(FloatVector2* p, FloatCircle* circle) {
        return overlapping(p->x, p->y, circle);
    }
    
    bool overlapping(float px, float py, float rx, float ry, float rw, float rh) {
        if (px < rx) return false;
        if (px >= rx+rw) return false;
        if (py < ry) return false;
        if (py >= ry+rh) return false;
        return true;
    }
    bool overlapping(float px, float py, FloatRect* rect) {
        return overlapping(px, py, rect->x, rect->y, rect->width, rect->height);
    }
    bool overlapping(FloatVector2* p, FloatRect* rect) {
        return overlapping(p->x, p->y, rect);
    }

    bool overlapping(float px, float py, FloatLine* line, float buffer) {
        float lineLen = distanceBetween(line->p1.x, line->p1.y, line->p2.x, line->p2.y);
        float d1 = distanceBetween(px ,py, line->p1.x, line->p1.y);
        float d2 = distanceBetween(px, py, line->p1.y, line->p2.y);

        if (d1+d2 >= lineLen-buffer && d1+d2 <= lineLen+buffer) {
            return true;
        }
        return false;
    }
    bool overlapping(float px, float py, FloatLine* line) {
        return overlapping(px, py, line, 0.1);
    }

    bool overlapping(IntRect* rect1, IntRect* rect2) {
        bool overlapX = abs((rect1->x + rect1->width/2.0) - (rect2->x + rect2->width/2.0)) <= (rect1->width/2.0 + rect2->width/2.0);
        bool overlapY = abs((rect1->y + rect1->height/2.0) - (rect2->y + rect2->height/2.0)) <= (rect1->height/2.0 + rect2->height/2.0);
        return overlapX && overlapY;
    }
    bool overlapping(FloatRect* rect1, FloatRect* rect2) {
        bool overlapX = abs((rect1->x + rect1->width/2.0) - (rect2->x + rect2->width/2.0)) <= (rect1->width/2.0 + rect2->width/2.0);
        bool overlapY = abs((rect1->y + rect1->height/2.0) - (rect2->y + rect2->height/2.0)) <= (rect1->height/2.0 + rect2->height/2.0);
        return overlapX && overlapY;
    }
    bool overlapping(FloatRect* rect, FloatCircle* circle) {
        float cx = circle->x;
        float cy = circle->y;

        if (cx < rect->x) cx = rect->x;
        if (cx > rect->x + rect->width) cx = rect->x + rect->width;
        if (cy < rect->y) cy = rect->y;
        if (cy > rect->y + rect->height) cy = rect->y + rect->height;
        if (Amara::distanceBetween(cx, cy, circle->x, circle->y) <= circle->radius) {
            return true;
        }
        return false;
    }
    bool overlapping(FloatCircle* circle, FloatRect* rect) {
        return Amara::overlapping(rect, circle);
    }
    bool overlapping(FloatCircle* circle1, FloatCircle* circle2) {
        return Amara::distanceBetween(circle1->x, circle1->y, circle2->x, circle2->y) < (circle1->radius + circle2->radius);
    }

    bool overlapping(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
        if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
            return true;
        }
        return false;
    }
    bool overlapping(FloatLine* line1, FloatLine* line2) {
        return overlapping(
            line1->p1.x, line1->p1.y, line1->p2.x, line1->p2.y,
            line2->p1.x, line2->p1.y, line2->p2.x, line2->p2.y
        );
    }
    bool overlapping(FloatLine* line, FloatRect* rect) {
        FloatLine rectLine = {{rect->x, rect->y},{rect->x+rect->width, rect->y}};
        if (overlapping(line, &rectLine)) return true;
        rectLine = {{rect->x, rect->y},{rect->x, rect->y+rect->height}};
        if (overlapping(line, &rectLine)) return true;
        rectLine = {{rect->x+rect->width, rect->y},{rect->x+rect->width, rect->y+rect->height}};
        if (overlapping(line, &rectLine)) return true;
        rectLine = {{rect->x, rect->y+rect->height},{rect->x+rect->width, rect->y+rect->height}};
        return false;
    }
    bool overlapping(FloatRect* rect, FloatLine* line) {
        return overlapping(line, rect);
    }
    bool overlapping(FloatLine* line, FloatCircle* circle) {
        if (overlapping(&line->p1, circle) || overlapping(&line->p2, circle)) return true;

        float distX = line->p1.x - line->p2.x;
        float distY = line->p1.y - line->p2.y;
        float len = sqrt((distX*distX) + (distY*distY));

        float dot = ( ((circle->x-line->p1.x)*(line->p2.x-line->p1.x)) + ((circle->y-line->p1.y)*(line->p2.y-line->p1.y)) ) / pow(len,2);

        float closestX = line->p1.x + (dot * (line->p2.x-line->p1.x));
        float closestY = line->p1.y + (dot * (line->p2.y-line->p1.y));
        
        FloatLine closeLine = { line->p1.x, line->p1.y, line->p2.x, line->p2.y };
        bool onSegment = overlapping(closestX, closestY, &closeLine);
        if (!onSegment) return false;

        distX = closestX - circle->x;
        distY = closestY - circle->y;
        float distance = sqrt( (distX*distX) + (distY*distY) );

        if (distance <= circle->radius) {
            return true;
        }
        return false;
    }
    bool overlapping(FloatCircle* circle, FloatLine* line) {
        return overlapping(line, circle);
    }

    int getOffsetX(Amara::Direction dir) {
        switch (dir) {
            case Up:
                return 0;
                break;
            case UpLeft:
                return -1;
                break;
            case UpRight:
                return 1;
                break;
            case Down:
                return 0;
                break;
            case DownLeft:
                return -1;
                break;
            case DownRight:
                return 1;
                break;
            case Left:
                return -1;
                break;
            case Right:
                return 1;
                break;
        }

        return 0;
    }

    int getOffsetY(Amara::Direction dir) {
        switch (dir) {
            case Up:
                return -1;
                break;
            case UpLeft:
                return -1;
                break;
            case UpRight:
                return -1;
                break;
            case Down:
                return 1;
                break;
            case DownLeft:
                return 1;
                break;
            case DownRight:
                return 1;
                break;
            case Left:
                return 0;
                break;
            case Right:
                return 0;
                break;
        }
        return 0;
    }

    Amara::Direction flipDirection(Amara::Direction dir, bool diagonals) {
        std::vector<Amara::Direction> list = (diagonals) ? DirectionsInOrder : CardinalDirections;

        for (int i = 0; i < list.size(); i++) {
            if (list[i] == dir) {
                int newIndex = i + list.size()/2;
                newIndex = newIndex % list.size();
                return list[newIndex];
            }
        }
        
        return dir;
    }

    Amara::Direction flipDirection(Amara::Direction dir) {
        return flipDirection(dir, true);
    }

    Amara::Direction turnDirection(Amara::Direction dir, std::vector<Direction> list, int turn) {
        int num = 0;
        for (int i = 0; i < list.size(); i++) {
            if (list[i] == dir) {
                num = i;
                break;
            }
        }
        num += turn;
        num = num % list.size();
        if (num < 0) num += list.size();

        return list[num];
    }
    Amara::Direction turnDirection(Amara::Direction dir, int turn) {
        return turnDirection(dir, DirectionsInOrder, turn);
    }

    Amara::Direction directionBetween(int x1, int y1, int x2, int y2, std::vector<Amara::Direction> list) {
        if (x1 == x2 && y1 == y2) {
            return NoDir;
        }

        double angle = atan2(y2 - y1, x2 - x1);
        int dirNum = floor(fmod(round(angle/(2*M_PI/list.size())), list.size()));
        if (dirNum < 0) {
            dirNum += list.size();
        }
        Amara::Direction direction = list[dirNum];

        return direction;
    }
	Amara::Direction directionBetween(int x1, int y1, int x2, int y2) {
        return directionBetween(x1, y1, x2, y2, DirectionsInOrder);
    }
	Amara::Direction directionBetween(IntVector2 p1, IntVector2 p2, std::vector<Amara::Direction> list) {
        return directionBetween(p1.x, p1.y, p2.x, p2.y, list);
    }
    Amara::Direction directionBetween(IntVector2 p1, IntVector2 p2) {
        return directionBetween(p1.x, p1.y, p2.x, p2.y);
    }

	Amara::Direction directionFromString(std::string dir) {
		if (dir.compare("up") == 0) return Up;
		if (dir.compare("down") == 0) return Down;
		if (dir.compare("left") == 0) return Left;
		if (dir.compare("right") == 0) return Right;
		if (dir.compare("upLeft") == 0) return UpLeft;
		if (dir.compare("upRight") == 0) return UpRight;
		if (dir.compare("downLeft") == 0) return DownLeft;
		if (dir.compare("downRight") == 0) return DownRight;
		return NoDir;
	}
	std::string getStringFromDirection(Amara::Direction dir) {
		switch (dir) {
			case Up:
				return "up";
				break;
			case Down:
				return "down";
				break;
			case Left:
				return "left";
				break;
			case Right:
				return "right";
				break;
			case UpLeft:
				return "upLeft";
				break;
			case UpRight:
				return "upRight";
				break;
			case DownLeft:
				return "downLeft";
				break;
			case DownRight:
				return "downRight";
				break;
		}
		return "noDir";
	}

    Amara::IntVector2 directionVector(Amara::Direction dir) {
        return { getOffsetX(dir), getOffsetY(dir) };
    }
    Amara::IntVector2 flipVector(Amara::IntVector2 v) {
        return { -v.x, -v.y };
    }
    float angleFromDirection(Direction dir) {
        int offsetX = getOffsetX(dir);
        int offsetY = getOffsetY(dir);
        return angleBetween(0, 0, offsetX, offsetY);
    }
}

#endif
