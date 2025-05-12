#pragma once
#include "circle.h"
#include "player.h"

class enemy : public circle {
public:
    float speed;

    enemy(float x1, float y1, int size1, float speed1, COLORREF color1);

    void move(float deltaTime, circle* food, int foodCount, player& hero);
    void update(float x1, float y1, int size1, float speed1);

private:
    float dirX, dirY;
    float directionTimer;
    float directionChangeInterval;
};