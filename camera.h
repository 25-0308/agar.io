#pragma once
#include <windows.h>

class Camera {
public:
    enum CameraMode {
        STATIC_VIEW,
        FOLLOW_PLAYER
    };

    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }

    CameraMode getCameraMode() const { return mode; }

    void calculateOffset(float heroX, float heroY, float& offsetX, float& offsetY)const;
    void handleInput(WPARAM wParam);

    float getScale() const { return scale; }

    void setCameraMode(CameraMode newMode) {
        mode = newMode;
		if (mode == STATIC_VIEW) {
			scale = 1.0f;
		}
		else if (mode == FOLLOW_PLAYER) {
			scale = 2.0f;
		}  
    }
private:
    Camera() : mode(STATIC_VIEW), screenWidth(1920.0f), screenHeight(1040.0f), scale(1.0f) {}

    CameraMode mode;
    const float screenWidth;
    const float screenHeight;
    float scale;
};