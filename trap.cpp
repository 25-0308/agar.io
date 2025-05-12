#include "trap.h"
#include <math.h>
#include <random>
#include "Camera.h"

static std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
static std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265359f);

trap::trap(float x1, float y1, int size1, float speed1, COLORREF color1)
    : circle(x1, y1, size1, static_cast<int>(speed1), color1),
    speed(speed1), dirX(0.0f), dirY(0.0f),
    directionTimer(0.0f), directionChangeInterval(10.0f) {
    float angle = angleDist(mtRand);
    dirX = cos(angle);
    dirY = sin(angle);
}

void trap::move(float deltaTime) {
    directionTimer += deltaTime;

    if (directionTimer >= directionChangeInterval) {
        float angle = angleDist(mtRand);
        dirX = cos(angle);
        dirY = sin(angle);
        directionTimer = 0.0f;
    }

    float moveSpeed = speed * 50.0f;
    x += dirX * moveSpeed * deltaTime;
    y += dirY * moveSpeed * deltaTime;

    x = max(static_cast<float>(size), min(1920.0f - static_cast<float>(size), x));
    y = max(static_cast<float>(size), min(1040.0f - static_cast<float>(size), y));
}

void trap::update(float x1, float y1, int size1, float speed1) {
    x = x1;
    y = y1;
    size = size1;
    speed = speed1;
}