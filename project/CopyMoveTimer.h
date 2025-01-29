#pragma once
#include <cstdio>
#include <string>
#include <chrono>
#include "WinApp.h"
#include "DirectXCommon.h"

class CopyMoveTimer {
public:
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	static void CopyTime(const std::string& src);

	static void MoveTime(std::string&& src);

private:
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
};
