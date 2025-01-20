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

	// Getter
	HWND GetHwnd()const { return hwnd; }

	// Getter
	HINSTANCE GetHInstance()const { return wc.hInstance; }

	// 終了
	void Finalize();

	// メッセージ処理
	bool ProcessMessage();

public:
	// クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:
	HWND hwnd = nullptr;
	//ウィンドウクラスの設定
	WNDCLASS wc{};
};