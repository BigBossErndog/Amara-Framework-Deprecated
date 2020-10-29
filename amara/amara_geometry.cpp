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
