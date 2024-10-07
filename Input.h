#pragma once
#include <Windows.h>
#define DIRECTINPUT_VERSION   0x0800
#include <dinput.h>
#include <wrl.h>

// 入力
class Input {
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	// 初期化
	void Initialize(HINSTANCE hInstance, HWND hdnw);
	// 更新
	void Update();

private:
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8>  keyboard;
};