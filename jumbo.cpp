#include "jumbo.h"
#include <math.h>
#include <windows.h>

std::mt19937 jumbo::mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_real_distribution<float> jumbo::angleDist(0.0f, 2.0f * 3.14159265359f);

void jumbo::move(float deltaTime, float heroX, float heroY) {
    directionTimer += deltaTime;

    float dx = heroX - x;
    float dy = heroY - y;
    float distance = sqrt(dx * dx + dy * dy);
    float followThreshold = 200.0f;

    float smoothingFactor = (distance < followThreshold) ? 2.0f - pow(0.1f, deltaTime) : 2.0f - pow(0.1f, deltaTime);
    float currentAngle = atan2(dirY, dirX);

    if (distance < followThreshold && distance > 0.1f) {
        targetAngle = atan2(dy, dx);
        currentAngle = currentAngle + smoothingFactor * (targetAngle - currentAngle);
    }
    else {
        if (directionTimer >= directionChangeInterval) {
            std::uniform_real_distribution<float> limitedAngle(targetAngle - 1.5708f, targetAngle + 1.5708f);
            targetAngle = limitedAngle(mtRand);
            if (targetAngle < 0) targetAngle += 2 * 3.14159f;
            if (targetAngle >= 2 * 3.14159f) targetAngle -= 2 * 3.14159f;
            directionTimer = 0.0f;
        }
        currentAngle = currentAngle + smoothingFactor * (targetAngle - currentAngle);
    }

    dirX = cos(currentAngle);
    dirY = sin(currentAngle);

    float speedVariation = 0.95f + 0.1f * (angleDist(mtRand) / 6.2832f);
    float moveSpeed = (distance < followThreshold) ? speed * 120.0f : speed * 80.0f * speedVariation;
    x += dirX * moveSpeed * deltaTime;
    y += dirY * moveSpeed * deltaTime;

    bool hitBoundary = false;
    static float boundaryCooldown = 0.0f;
    boundaryCooldown -= deltaTime;
    if (boundaryCooldown <= 0.0f) {
        if (x < static_cast<float>(size)) {
            x = static_cast<float>(size);
            std::uniform_real_distribution<float> rightAngle(0.7854f, 2.3562f);
            targetAngle = rightAngle(mtRand);
            hitBoundary = true;
            boundaryCooldown = 2.0f;
        }
        else if (x > 1920.0f - static_cast<float>(size)) {
            x = 1920.0f - static_cast<float>(size);
            std::uniform_real_distribution<float> leftAngle(3.9270f, 5.4978f);
            targetAngle = leftAngle(mtRand);
            hitBoundary = true;
            boundaryCooldown = 2.0f;
        }
        if (y < static_cast<float>(size)) {
            y = static_cast<float>(size);
            std::uniform_real_distribution<float> downAngle(0.7854f, 2.3562f);
            targetAngle = downAngle(mtRand);
            hitBoundary = true;
            boundaryCooldown = 2.0f;
        }
        else if (y > 1040.0f - static_cast<float>(size)) {
            y = 1040.0f - static_cast<float>(size);
            std::uniform_real_distribution<float> upwardAngle(-0.7854f, 0.7854f);
            targetAngle = upwardAngle(mtRand);
            hitBoundary = true;
            boundaryCooldown = 2.0f;
        }
    }
    x = std::clamp(x, static_cast<float>(size), 1920.0f - static_cast<float>(size));
    y = std::clamp(y, static_cast<float>(size), 1040.0f - static_cast<float>(size));

    if (hitBoundary) {
        currentAngle = targetAngle;
        dirX = cos(currentAngle);
        dirY = sin(currentAngle);
    }
}

void jumbo::update(float x1, float y1, int size1, float speed1) {
    x = x1;
    y = y1;
    size = size1;
    speed = speed1;
    targetAngle = angleDist(mtRand);
    dirX = cos(targetAngle);
    dirY = sin(targetAngle);
    directionTimer = 0.0f;
    directionChangeInterval = 5.0f;
}

void jumbo::render(HWND hwnd, HDC hdc,float camerax, float cameray) {
    POINT point[3];
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, hBrush);
	x = x - camerax;
	y = y - cameray;
    float angle = atan2(dirY, dirX);
    point[0].x = static_cast<int>(x + size * cos(angle));
    point[0].y = static_cast<int>(y + size * sin(angle));
    point[1].x = static_cast<int>(x + size * cos(angle + 2.0944f));
    point[1].y = static_cast<int>(y + size * sin(angle + 2.0944f));
    point[2].x = static_cast<int>(x + size * cos(angle - 2.0944f));
    point[2].y = static_cast<int>(y + size * sin(angle - 2.0944f));

    Polygon(hdc, point, 3);
    SelectObject(hdc, oldbrush);
    DeleteObject(hBrush);
}