#include "Camera.h"
#include <algorithm>
#include <windows.h>

void Camera::calculateOffset(float heroX, float heroY, float& offsetX, float& offsetY) const {
    static bool initialized = false;
    static float currentOffsetX = 0.0f, currentOffsetY = 0.0f;

    if (!initialized) {
        currentOffsetX = heroX - screenWidth / 2.0f;
        currentOffsetY = heroY - screenHeight / 2.0f;
        initialized = true;
    }

    if (mode == FOLLOW_PLAYER) {
        float mapWidth = 1920.0f;
        float mapHeight = 1040.0f;

        float targetX = heroX - screenWidth / 2.0f;
        float targetY = heroY - screenHeight / 2.0f;

        targetX = std::clamp(targetX, 0.0f, mapWidth - screenWidth);
        targetY = std::clamp(targetY, 0.0f, mapHeight - screenHeight);

        currentOffsetX += (targetX - currentOffsetX) * 0.1f;
        currentOffsetY += (targetY - currentOffsetY) * 0.1f;
        offsetX = currentOffsetX;
        offsetY = currentOffsetY;
    }
    else {
        offsetX = currentOffsetX = 0.0f;
        offsetY = currentOffsetY = 0.0f;
    }
}

void Camera::handleInput(WPARAM wParam) {
    switch (wParam) {
    case 0x31: // Ű '1'
        setCameraMode(FOLLOW_PLAYER);
        break;
    case 0x32: // Ű '2'
        setCameraMode(STATIC_VIEW);
        break;
    }
}