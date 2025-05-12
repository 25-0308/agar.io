#include "ui.h"
#include <Windows.h>
#include "resource.h"

ui::ui(int time1, int size1, int count1) {
    init(time1, size1, count1);
}

void ui::init(int time1, int size1, int count1) {
    time = time1;
    size = size1;
    count = count1;
}

void ui::update(int time1, int size1, int count1) {
    time = time1;
    size = size1;
    count = count1;
}

void ui::render(HWND hwnd, HDC hdc) {
    TCHAR buffer1[100];
    TCHAR buffer2[100];
    wsprintfW(buffer1, L"Time : %ds", time);
    wsprintfW(buffer2, L"SIZE : %d food : %d", size, count);
    TextOutW(hdc, 0, 0, buffer1, lstrlenW(buffer1));
    TextOutW(hdc, 0, 20, buffer2, lstrlenW(buffer2));
}

void ui::score(HWND hwnd, HDC hdc, int score) {
    TCHAR buffer2[100];
    wsprintfW(buffer2, L"%d¡°", score);

    HFONT hFont = CreateFontW(
        150, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial"
    );
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SIZE textSize;
    GetTextExtentPoint32W(hdc, buffer2, lstrlenW(buffer2), &textSize);

    int screenWidth = 1920;
    int screenHeight = 1080;
    int x = (screenWidth - textSize.cx) / 2;  
    int y = (screenHeight - textSize.cy) / 2;

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255)); 

    TextOutW(hdc, x, y, buffer2, lstrlenW(buffer2));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}