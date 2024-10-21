#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"

// DirectX基盤
class DirectXCommon {
public:
	// 初期化
	void Initialize(WinApp* winApp);

	// デバイスの初期化
	void DeviceInitialize();

	// コマンド関係の初期化
	void CommandInitialize();

	// スワップチェーンの生成
	void SwapChainGenerate();

	// 深度バッファの生成
	void DepthBufferGenerate();

private:
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	WinApp* winApp = nullptr;
};