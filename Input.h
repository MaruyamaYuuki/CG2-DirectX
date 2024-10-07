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
    /// <summary>
    /// キーの押下をチェック
    /// </summary>
	/// <pragma name="keyNumber">キー番号( DIK_ 等)</pragma>
	/// <retuns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

private:
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8>  keyboard;
	// 全キーの状態
	BYTE key[256] = {};
};