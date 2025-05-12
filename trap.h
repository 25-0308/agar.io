#pragma once
#include "circle.h"

class trap : public circle {
public:
    float speed;

    trap(float x1, float y1, int size1, float speed1, COLORREF color1);

    void move(float deltaTime);

    void update(float x1, float y1, int size1, float speed1);

private:
    float dirX, dirY;
    float directionTimer;
    float directionChangeInterval;
};