#pragma once
#include <fstream>
#include <d3d12.h>
#include <wrl.h>
#include <chrono>
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex//d3dx12.h"

class DirectXCommon;

// テスクチャーマネージャー
class TextureManager {
private:
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

	// テスクチャ1枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	// テスクチャデータ
	std::vector<TextureData> textureDatas;

	DirectXCommon* dxCommon_ = nullptr;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;
public:
	// 初期化
	void Initialize();

	// シングルトンインスタンスの取得
	static TextureManager* GetInstance();

	// 終了
	void Finalize();

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャファイルのパス</param>
	void LoadTexture(const std::string& filePath);

	// SRVインデックスの開始番号
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	// テスクチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureInex);
};