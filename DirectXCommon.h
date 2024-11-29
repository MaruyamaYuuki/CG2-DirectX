#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <format>
#include "Logger.h"
#include "StringUtility.h"
#include "array"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex//d3dx12.h"
#include <vector>
#include <dxcapi.h>
#include <chrono>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "dxcompiler.lib")

// DirectX基盤
class DirectXCommon {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();

	// getter
	ID3D12Device* GetDevice()const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandlist()const { return commandList.Get(); }

	// シェーダーコンパイル
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);

	/// <summary>
	/// バッファリソースの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	///<summary>
	/// テクスチャリソース生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	/// <summary>
	/// テクスチャデータの転送
	/// </summary>
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <pragma name="finePath">テクスチャファイルのパス</param>
	/// <returns>画像イメージデータ</returns>
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	/// <summary>
	/// DepthStencilTextureの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const int32_t& width, const int32_t& height);

	/// <summary>
	/// デスクリプタヒープの生成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		Microsoft::WRL::ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, const UINT& numDescriptors, bool shaderVisible);

    /// <summary>
    /// 指定番号のCPUデスクリプタハンドルを取得する
    /// </summary>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const uint32_t& descriptorSize, const uint32_t& index);

    /// <summary>
    /// 指定番号のGPUデスクリプタハンドルを取得する
    /// </summary>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const uint32_t& descriptorSize, const uint32_t& index);

    /// <summary>
    /// SRVのしてh番号のCPUデスクリプタハンドルを取得する
    /// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

	/// <summary>
    /// SRVのしてh番号のGPUデスクリプタハンドルを取得する
    /// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	// 最大SRV数（最大テクスチャ枚数）
	static const uint32_t kMaxSRVCount;

private:
	/// <summary>
	/// デバイスの初期化
	/// </summary>
	void DeviceInitialize();

	/// <summary>
	/// コマンド関係の初期化
	/// </summary>
	void CommandInitialize();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void SwapChainGenerate();

	/// <summary>
	/// 深度バッファの生成
	/// </summary>
	void DepthBufferGenerate();

	/// <summry>
	/// 各種デスクリプタヒープの生成
	/// </summary>
	void DescriptorHeapGenerate();

	/// <summary>
	/// レンダーターゲットビューの初期化
	/// </summary>
	void RenderTargetViewInitialize();

    /// <summary>
    /// 深度ステンシルビューの初期化
    /// </summary>
	void DepthStencilViewInitialize();
	
	/// <summary>
	/// フェンスの生成
	/// </summary>
	void FenceInitialize();

	/// <summary>
	/// ビューポート矩形の生成
	/// </summary>
	void ViewportInitialize();

	/// <summary>
	/// シザリング矩形の生成
	/// </summary>
	void ScissoringInitialize();

	/// <summary>
	/// DXCコンパイラの生成
	/// </summary>
	void DXCGenerate();

	/// <summary>
	/// ImGuiの生成
	/// </summary>
	void ImGuiInitialize();



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
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	WinApp* winApp = nullptr;

	// RTV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
	// SRV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;
	// DSV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
	// 各種DescroptorSize
	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;

	// SRVのデスクリプタハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// スワップチェーンリソース
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource;

	// フェンス
	UINT64 fenceValue = 0;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	HANDLE fenceEvent;

	// ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> temporaryResources_;

	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	// 記録時間
	std::chrono::steady_clock::time_point reference_;



private:
	// FPS固定初期化
	void InitialiseFixFPS();
	/// FPS固定更新
	void UpdateFixFPS();
};