#pragma once
#include "circle.h"

class player : public circle {
public:
    float speed;

    player(float x1, float y1, int size1, float speed1, COLORREF color1)
        : circle(x1, y1, size1, static_cast<int>(speed1), color1), speed(speed1) {
    }

    void move(int mx, int my, float deltaTime);
    void update(float x1, float y1, int size1, float speed1);
};