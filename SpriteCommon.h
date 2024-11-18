#pragma once
#include "DirectXCommon.h"

// スプライト共通部
class SpriteCommon {
public:
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// 共通描画設定
	void SettingCommonDraw();

	DirectXCommon* getDxCommon()const { return dxCommon_; }

private:
	// ルートシグネチャの作成
	void createRootSignature();
	// グラフィックスパイプラインの生成
	void GeneratePipeline();

private:
	DirectXCommon* dxCommon_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
};