#pragma once
#include "circle.h"
#include <random>

class jumbo : public circle {
public:
    float speed;

    jumbo(float x1, float y1, int size1, float speed1, COLORREF color1)
        : circle(x1, y1, size1, static_cast<int>(speed1), color1), speed(speed1) {
        directionTimer = 0.0f;
        directionChangeInterval = 2.5f;
        targetAngle = angleDist(mtRand);
        dirX = cos(targetAngle);
        dirY = sin(targetAngle);
    }

    void move(float deltaTime, float heroX, float heroY);
    void update(float x1, float y1, int size1, float speed1);
    void render(HWND hwnd, HDC hdc, float camerax, float cameray);

private:
    float dirX, dirY;
    float directionTimer;
    float directionChangeInterval;
    float targetAngle;
    static std::mt19937 mtRand;
    static std::uniform_real_distribution<float> angleDist;
};