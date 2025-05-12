#include "enemy.h"
#include <math.h>
#include <random>
#include "Camera.h"

extern circle food[30];

static std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
static std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265359f);

enemy::enemy(float x1, float y1, int size1, float speed1, COLORREF color1)
    : circle(x1, y1, size1, static_cast<int>(speed1), color1),
    speed(speed1), dirX(0.0f), dirY(0.0f),
    directionTimer(0.0f), directionChangeInterval(2.0f) {
    float angle = angleDist(mtRand);
    dirX = cos(angle);
    dirY = sin(angle);
}

void enemy::move(float deltaTime, circle* food, int foodCount, player& hero) {
    float dxHero = hero.x - x;
    float dyHero = hero.y - y;
    float distanceToHero = sqrt(dxHero * dxHero + dyHero * dyHero);
    const float chaseRange = size * 5.0f;

    if (distanceToHero < chaseRange && distanceToHero > 0.1f) {
        dirX = dxHero / distanceToHero;
        dirY = dyHero / distanceToHero;
    }
    else {
        float minDistance = 1e9f;
        float targetX = 0.0f, targetY = 0.0f;
        bool foundFood = false;

        for (int i = 0; i < foodCount; i++) {
            float dx = food[i].x - x;
            float dy = food[i].y - y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < minDistance) {
                minDistance = distance;
                targetX = food[i].x;
                targetY = food[i].y;
                foundFood = true;
            }
        }

        if (foundFood && minDistance > 0.1f) {
            float dx = targetX - x;
            float dy = targetY - y;
            float distance = sqrt(dx * dx + dy * dy);
            dirX = dx / distance;
            dirY = dy / distance;
        }
        else {
            directionTimer += deltaTime;
            if (directionTimer >= directionChangeInterval) {
                float angle = angleDist(mtRand);
                dirX = cos(angle);
                dirY = sin(angle);
                directionTimer = 0.0f;
            }
        }
    }

    float moveSpeed = speed * 100.0f;
    x += dirX * moveSpeed * deltaTime;
    y += dirY * moveSpeed * deltaTime;

    x = max(static_cast<float>(size), min(1920.0f - static_cast<float>(size), x));
    y = max(static_cast<float>(size), min(1040.0f - static_cast<float>(size), y));
}

void enemy::update(float x1, float y1, int size1, float speed1) {
    x = x1;
    y = y1;
    size = size1;
    speed = speed1;
    if (size > 250) {
        size = 250;
    }
    if (speed < 0.1f) {
        speed = 0.1f;
    }
}