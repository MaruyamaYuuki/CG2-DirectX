#pragma once
#include <Windows.h>

// WindowsAPI
class WinApp {
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// ウィンドウプロシージャ
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};