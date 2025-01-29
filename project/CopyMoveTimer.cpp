#include "CopyMoveTimer.h"
#include "Logger.h"

void CopyMoveTimer::Initialize(WinApp* winApp, DirectXCommon* dxCommon)
{
	this->winApp = winApp;
	this->dxCommon = dxCommon;
}


void CopyMoveTimer::CopyTime(const std::string& src)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::string dest = src; // コピー処理
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    // ログに出力
    Logger::Log("Copy: " + std::to_string(duration) + " ns\n");
}

void CopyMoveTimer::MoveTime(std::string&& src)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::string dest = std::move(src); // ムーブ処理
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    // ログに出力
    Logger::Log("Move: " + std::to_string(duration) + " ns\n");
}
