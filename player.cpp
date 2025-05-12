#include "player.h"
#include <math.h>
#include "Camera.h"

void player::move(int mx, int my, float deltaTime) {
    float dx = static_cast<float>(mx) - x;
    float dy = static_cast<float>(my) - y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance > 0.1f) {
        float dirX = dx / distance;
        float dirY = dy / distance;
        float moveSpeed = speed * 100.0f;
        x += dirX * moveSpeed * deltaTime;
        y += dirY * moveSpeed * deltaTime;
    }

    x = max(static_cast<float>(size), min(1920.0f - static_cast<float>(size), x));
    y = max(static_cast<float>(size), min(1040.0f - static_cast<float>(size), y));
}

void player::update(float x1, float y1, int size1, float speed1) {
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