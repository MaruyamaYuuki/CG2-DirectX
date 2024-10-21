#pragma once
#include <Windows.h>
#include <cstdint>

// WindowsAPI
class WinApp {
public:
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	// 初期化
	void Initialize();

	// 更新
	void Update();



};