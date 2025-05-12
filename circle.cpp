#include "circle.h"
#include <Windows.h>

circle::circle() {
    x = 0.0f;
    y = 0.0f;
    size = 0;
    speed = 0;
    color = RGB(0, 0, 0);
}

circle::circle(float x1, float y1, int size1, int speed1, COLORREF color1) {
    init(x1, y1, size1, speed1, color1);
}

void circle::init(float x1, float y1, int size1, int speed1, COLORREF color1) {
    x = x1;
    y = y1;
    size = size1;
    speed = speed1;
    color = color1;
}

void circle::render(HWND hwnd, HDC hdc,float camerax,float cameray) {
	x = x - camerax;
    y = y - cameray;
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, static_cast<int>(x) - size, static_cast<int>(y) - size, static_cast<int>(x) + size, static_cast<int>(y) + size);
    SelectObject(hdc, oldbrush);
    DeleteObject(hBrush);
}

void circle::update(float x1, float y1, int size1, COLORREF color1) {
    x = x1;
    y = y1;
    size = size1;
    color = color1;
}