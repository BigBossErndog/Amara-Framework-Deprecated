#pragma once
#ifndef AMARA_GEOMETRY
#define AMARA_GEOMETRY

#include "amara.h"

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

    bool overlapping(float px, float py, FloatCircle* circle) {
        return (Amara::distanceBetween(px, py, circle->x, circle->y) <= circle->radius);
    }
    bool overlapping(FloatVector2* p, FloatCircle* circle) {
        return overlapping(p->x, p->y, circle);
    }
    
    bool overlapping(float px, float py, FloatRect* rect) {
        if (px < rect->x) return false;
        if (px > rect->x+rect->width) return false;
        if (py < rect->y) return false;
        if (py > rect->y+rect->width) return false;
        return true;
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
        bool overlapX = abs((rect1->x + rect1->width/2.0) - (rect2->x + rect2->width/2.0)) < (rect1->width/2.0 + rect2->width/2.0);
        bool overlapY = abs((rect1->y + rect1->height/2.0) - (rect2->y + rect2->height/2.0)) < (rect1->height/2.0 + rect2->height/2.0);
        return overlapX && overlapY;
    }
    bool overlapping(FloatRect* rect1, FloatRect* rect2) {
        bool overlapX = abs((rect1->x + rect1->width/2.0) - (rect2->x + rect2->width/2.0)) < (rect1->width/2.0 + rect2->width/2.0);
        bool overlapY = abs((rect1->y + rect1->height/2.0) - (rect2->y + rect2->height/2.0)) < (rect1->height/2.0 + rect2->height/2.0);
        return overlapX && overlapY;
    }
    bool overlapping(FloatRect* rect, FloatCircle* circle) {
        float cx = circle->x;
        float cy = circle->y;

        if (cx < rect->x) cx = rect->x;
        if (cx > rect->x + rect->width) cx = rect->x + rect->width;
        if (cy < rect->y) cy = rect->y;
        if (cy > rect->y + rect->height) cy = rect->y + rect->height;
        if (Amara::distanceBetween(cx, cy, circle->x, circle->y) < circle->radius) {
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

    Amara::Direction turnDirection(Amara::Direction dir, bool diagonals) {
        std::vector<Amara::Direction> list = (diagonals) ? DirectionsInOrder : FourDirections;

        for (int i = 0; i < list.size(); i++) {
            if (list[i] == dir) {
                int newIndex = i + list.size()/2;
                newIndex = newIndex % list.size();
                return list[newIndex];
            }
        }
        
        return dir;
    }

    Amara::Direction turnDirection(Amara::Direction dir) {
        return turnDirection(dir, true);
    }

	Amara::Direction getDirection(std::string dir) {
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
}

#endif
