#include "Sprite.h"
#include "SpriteCommon.h"

void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	// 引数で受け取ってメンバ変数に記録する
	this->spriteCommon = spriteCommon;

	// VertexResouceを作る
	vertexResource = spriteCommon->getDxCommon()->CreateBufferResource(sizeof(VertexData) * 4);
	// IndexResourceを作る
	indexResource = spriteCommon->getDxCommon()->CreateBufferResource(sizeof(uint32_t) * 6);
	// VertexBufferViewを作成
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	// IndexBufferViewを作成
	// リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// アドレスを取得して書き込む
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));


}
