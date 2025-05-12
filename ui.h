#pragma once
#include <windows.h>

class ui {
public:
	int time;
	int size;
	int count;

	ui(int time1, int size1, int count1);

	void init(int time1, int size1, int count1);

	void update(int time1, int size1, int count1);

	void render(HWND hwnd, HDC hdc);

	void score(HWND hwnd, HDC hdc,int score);

};