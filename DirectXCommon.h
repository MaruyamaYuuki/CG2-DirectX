#pragma once
#include <string>

// DirectX基盤
class DirectXCommon {
public:
	// 初期化
	void Initialize();

	std::string ConvertString(const std::wstring& str);

	void Log(const std::string& message);
};