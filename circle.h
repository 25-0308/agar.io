#pragma once
#include <windows.h>

class circle {
public:
    float x = 0.0f;
    float y = 0.0f;
    int size = 0;
    int speed = 0;
    COLORREF color = RGB(0, 0, 0);

    circle();
    circle(float x1, float y1, int size1, int speed1, COLORREF color1);
    void init(float x1, float y1, int size1, int speed1, COLORREF color1);
    void render(HWND hwnd, HDC hdc,float camerax, float cameray);
    void update(float x1, float y1, int size1, COLORREF color1);
};