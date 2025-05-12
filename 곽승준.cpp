#include <Windows.h>
#include <mmsystem.h>
#include <random>
#include <cmath>
#include "circle.h"
#include "player.h"
#include "ui.h"
#include "enemy.h"
#include "trap.h"
#include "jumbo.h"
#include "Camera.h"
#include "resource.h"

#pragma comment(lib, "Winmm.lib")

#define default_size 20

std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_int_distribution<int> xxx(100, 1820);
std::uniform_int_distribution<int> yyy(100, 980);

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Agar.io";

int playtime = 0;
bool start = false;
bool follow = false;
bool finish = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

circle food[30];
player hero(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), default_size, 0.4f, RGB(146, 129, 205));
enemy* virus = new enemy(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), default_size, 0.4f, RGB(255, 0, 0));
trap* trap_ = new trap(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 65, 0.2f, RGB(0, 0, 255));
jumbo jumbo_(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 15, 0.4f, RGB(255, 255, 0));
ui UI(0, hero.size, 0);

void reset() {
    playtime += UI.time;
    hero.update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), default_size, 0.4f);
    virus->update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), default_size, 0.4f);
    trap_->update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 65, 0.2f);
    jumbo_.update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 15, 0.4f);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    for (int i = 0; i < 30; i++) {
        food[i].init(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 6, 0, RGB(0, 255, 0));
    }

    float initialOffsetX, initialOffsetY;
    Camera::getInstance().calculateOffset(hero.x, hero.y, initialOffsetX, initialOffsetY);

    HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(101));
    HBITMAP hBitmap2 = LoadBitmap(hInstance, MAKEINTRESOURCE(102)); 
    BITMAP bm, bm2;
    GetObject(hBitmap, sizeof(BITMAP), &bm);
    GetObject(hBitmap2, sizeof(BITMAP), &bm2);

    bool gameover = false;

    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
        0, 0, 1920, 1080, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    InvalidateRect(hWnd, NULL, TRUE);

    HDC hdc = GetDC(hWnd);
    SetTimer(hWnd, 1, 1000, NULL);

    HDC hBitmapDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(hBitmapDC, hBitmap);

    DWORD lastTime = timeGetTime();
    POINT mousePos = { 0, 0 };

    while (true) {
        if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
            if (Message.message == WM_QUIT) break;
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        else {
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 1920, 1080);
            HBITMAP oldMemBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
            HBRUSH whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
            RECT rect = { 0, 0, 1920, 1080 };
            FillRect(memDC, &rect, whiteBrush);

            if (start == true) {
                DWORD currentTime = timeGetTime();
                float deltaTime = (currentTime - lastTime) / 150.0f;
                deltaTime = min(deltaTime, 0.1f);
                lastTime = currentTime;

                GetCursorPos(&mousePos);
                ScreenToClient(hWnd, &mousePos);
                hero.move(mousePos.x, mousePos.y, deltaTime);
                virus->move(deltaTime, food, 30, hero);
                trap_->move(deltaTime);
                jumbo_.move(deltaTime, hero.x, hero.y);

                if (virus) {
                    float dx = hero.x - virus->x;
                    float dy = hero.y - virus->y;
                    float distance = sqrt(dx * dx + dy * dy);
                    float collisionDistance = static_cast<float>(hero.size + virus->size);

                    if (distance < collisionDistance) {
                        if (hero.size > virus->size) {
                            int virusSize = virus->size - 10;
                            float virusSpeed = virus->speed - 0.5f;
                            delete virus;
                            hero.update(hero.x, hero.y, hero.size + virusSize, hero.speed + virusSpeed);
                            virus = new enemy(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), default_size, 0.5f, RGB(255, 0, 0));
                        }
                        else if (hero.size < virus->size) {
                            gameover = true;
                        }
                    }
                }
                for (int i = 0; i < 30; i++) {
                    float dx = hero.x - food[i].x;
                    float dy = hero.y - food[i].y;
                    float distance = sqrt(dx * dx + dy * dy);
                    float collisionDistance = static_cast<float>(hero.size + food[i].size);

                    if (distance < collisionDistance) {
                        food[i].init(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 6, 0, RGB(0, 255, 0));
                        hero.update(hero.x, hero.y, hero.size + 1, hero.speed - 0.008f);
                        UI.count++;
                    }
                }
                for (int i = 0; i < 30; i++) {
                    float dx = virus->x - food[i].x;
                    float dy = virus->y - food[i].y;
                    float distance = sqrt(dx * dx + dy * dy);
                    float collisionDistance = static_cast<float>(virus->size + food[i].size);

                    if (distance < collisionDistance) {
                        food[i].init(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 6, 0, RGB(0, 255, 0));
                        virus->update(virus->x, virus->y, virus->size + 1, virus->speed - 0.008f);
                    }
                }

                float cameraOffsetX, cameraOffsetY;
                Camera::getInstance().calculateOffset(hero.x, hero.y, cameraOffsetX, cameraOffsetY);

                for (int i = 0; i < 30; i++) {
                    food[i].render(hWnd, memDC, cameraOffsetX, cameraOffsetY);
                }
                hero.render(hWnd, memDC, cameraOffsetX, cameraOffsetY);
                UI.render(hWnd, memDC);
                virus->render(hWnd, memDC, cameraOffsetX, cameraOffsetY);
                trap_->render(hWnd, memDC, cameraOffsetX, cameraOffsetY);
                jumbo_.render(hWnd, memDC, cameraOffsetX, cameraOffsetY);
            }
            else if (start == false) {
                StretchBlt(memDC, 0, 0, 1920, 1040, hBitmapDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
            }
            if (finish == true) {
                SelectObject(hBitmapDC, hBitmap2);
                StretchBlt(memDC, 0, 0, 1920, 1040, hBitmapDC, 0, 0, bm2.bmWidth, bm2.bmHeight, SRCCOPY);
                UI.score(hWnd, memDC, playtime);
                KillTimer(hWnd, 1);
                PostQuitMessage(0);
            }
            if (follow == false) BitBlt(hdc, 0, 0, 1920, 1080, memDC, 0, 0, SRCCOPY);
            else StretchBlt(hdc, 0, 0, 1920, 1040, memDC, static_cast<int>(hero.x) - hero.size * 8, static_cast<int>(hero.y) - hero.size * 8, hero.size * 16, hero.size * 16, SRCCOPY);

            SelectObject(memDC, oldMemBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
        }
    }

    SelectObject(hBitmapDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteObject(hBitmap2);
    DeleteDC(hBitmapDC);
    KillTimer(hWnd, 1);
    ReleaseDC(hWnd, hdc);
    delete virus;
    delete trap_;
    if(finish==true) Sleep(3000);
    return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    switch (iMessage) {
    case WM_KEYDOWN:
    {
        if (wParam == '1') {
            follow = true;
        }
        else if (wParam == '2') {
            follow = false;
        }
        break;
    }
    case WM_CHAR:
    {
        if (wParam == 'n' || wParam == 'N') {
            start = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == 'r' || wParam == 'R') {
            start = false;
            hero.update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), default_size, 0.4f);
            virus->update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), default_size, 0.4f);
            trap_->update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 65, 0.2f);
            jumbo_.update(static_cast<float>(xxx(mtRand)), static_cast<float>(yyy(mtRand)), 15, 0.4f);
            UI.time = 0;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == 'q' || wParam == 'Q') {
            finish = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }
    case WM_TIMER:
    {
        if (wParam == 1) {
            UI.time++;
            playtime++;
            UI.update(UI.time, hero.size, UI.count);
        }
        break;
    }
    case WM_PAINT:
    {
        HDC hDC = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}