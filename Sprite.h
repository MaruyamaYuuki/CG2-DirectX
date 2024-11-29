#pragma once
#include "MathFunctions.h"
#include <d3d12.h>
#include <wrl.h>
#include <chrono>

using namespace MathFunctions;

class SpriteCommon;
class TextureManager;

// スプライト
class Sprite {
public:
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

public:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
public:
	// 初期化
	void Initialize(SpriteCommon* spriteCommon, std::string textureFilePath);

	// 更新
	void Update();

	// 描画
	void Draw();

	// getter
	const Vector2& GetPosition()const { return position; }
	float GetRotation()const { return rotation; }
	const Vector4& GetColor()const { return materialData->color; }
	const Vector2& GetSize()const { return size; }
	// setter
	void SetPosition(const Vector2& posisiton) { this->position = posisiton; }
	void SetRoration(float rotation) { this->rotation = rotation; }
	void SetColor(const Vector4& color) { materialData->color = color; }
	void SetSize(const Vector2& size) { this->size = size; }

private:
	SpriteCommon* spriteCommon = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* transformationMatrixData;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	Vector2 position = { 0.0f,0.0f };
	float rotation = 0.0f;
	Vector2 size = { 120.0f,120.0f };

	// テクスチャ番号
	uint32_t textureIndex = 0;
};