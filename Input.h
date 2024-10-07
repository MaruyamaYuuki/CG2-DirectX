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

    /// <summary>
    /// キーのトリガーチェック
    /// </summary>
	/// <pragma name="keyNumber">キー番号( DIK_ 等)</pragma>
	/// <return>トリガーか<_resurn>
	bool TriggerKey(BYTE keyNumber);

private:
	// DirectInputのインスタンス
	ComPtr<IDirectInput8> directInput;
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8W>  keyboard;
	// 全キーの状態
	BYTE key[256] = {};
	// 全開の全キーの状態
	BYTE keyPre[256] = {};
};