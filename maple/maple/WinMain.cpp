#pragma once

#include "MyHeader.h"

void game_Init(MSG &Message);
void game_Progress();
void game_Release();
HDC return_dc();

LRESULT CALLBACK MyProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int _stdcall WinMain(HINSTANCE myins, HINSTANCE, PSTR, int) {

	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));

	wc.lpszClassName = "Maple";
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpfnWndProc = MyProc;
	RegisterClass(&wc);

	RECT rc;

	SetRect(&rc, 0, 0, WINSIZEX, WINSIZEY);
	

	int posX = (GetSystemMetrics(SM_CXSCREEN) - WINSIZEX )/ 2;
	
	int posY = (GetSystemMetrics(SM_CYSCREEN) - WINSIZEY) / 2;




	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, NULL);

	HWND hWnd = CreateWindow("Maple", "MY_Maple", WS_OVERLAPPEDWINDOW, posX, posY, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, myins, NULL);
	ShowWindow(hWnd, SW_SHOW);

	MSG msg;

	memset(&msg, 0, sizeof(MSG));
	game_Init(msg);

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		game_Progress();
	}

	game_Release();
	return 0;
}

LRESULT CALLBACK MyProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static POINT pt1 = { 0, 0 }, pt2 = { 0, 0 };
	static HDC _dc = GetDC(hWnd);
	static bool button_check = false;
	switch (msg) {
	case WM_KEYDOWN:
		switch (wParam){
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;


	}


	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
