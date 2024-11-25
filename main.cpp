#include <cmath>
#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
#include "ResourceObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "D3DResourceLeakChecker.h"
#include "Sprite.h"
#include "SpriteCommon.h"

#pragma comment (lib, "dxcompiler.lib")

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};
struct MaterialData {
	std::string textureFilePath;
};
struct ModelData {
	std::vector<Sprite::VertexData>vertices;
	MaterialData material;
};

MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	// 必要な変数の宣言とファイルを開く
	MaterialData materialData;
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	// ファイルを読み、MaterialDataを構築
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}

ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	// 1.必要な変数の宣言
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	// 2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	// ファイルを読み、ModelDataを構築
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			Sprite::VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices [0]- 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			// 頂点を逆順で登録することで、回り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			// materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名をファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
	return modelData;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakChek;

	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	SpriteCommon* spriteCommon = nullptr;

	// WinAppの初期化
	winApp = new WinApp();
	winApp->Initialize();
	// DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	// 入力の初期化
	input = new Input();
	input->Initialize(winApp);
	// スプライト共通部の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon);

	// WVP用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = dxCommon->CreateBufferResource(sizeof(Sprite::TransformationMatrix));
	// データを書き込む
	Sprite::TransformationMatrix* wvpData = nullptr;
	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	// 単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();

	// マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = dxCommon->CreateBufferResource(sizeof(Sprite::Material));
	// マテリアル用にデータを書き込む
	Sprite::Material* materialData = nullptr;
	// 書き込むためのアドレスと取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	Vector4 color{ 1.0f,1.0f,1.0f,1.0f };
	materialData->color = color;
	materialData->enableLighting = 1;
	materialData->uvTransform = MakeIdentity4x4();

	// 平行光源用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = dxCommon->CreateBufferResource(sizeof(DirectionalLight));
	DirectionalLight* directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	DirectionalLight light{ {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };
	directionalLightData->color = light.color;
	directionalLightData->direction = light.direction;
	directionalLightData->intensity = light.intensity;

    /*// 頂点リソースを作る
	float pi = float(M_PI);
	// 球の分割数
	const uint32_t kSubdivision = 160;
	const uint32_t kDivision = (kSubdivision + 1) * (kSubdivision + 1);
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device, sizeof(VertexData) * kDivision);

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * kDivision;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 経度分割1つ分の角度φ
	const float kLonEvery = 2.0f * pi / float(kSubdivision);
	// 緯度分割1つ分の角度θ
	const float kLatEvery = pi / float(kSubdivision);

	// 頂点データを作成
	for (uint32_t latIndex = 0; latIndex <= kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		for (uint32_t lonIndex = 0; lonIndex <= kSubdivision; ++lonIndex) {
			float lon = kLonEvery * lonIndex;
			uint32_t index = latIndex * (kSubdivision + 1) + lonIndex;

			vertexData[index].position.x = cos(lat) * cos(lon);
			vertexData[index].position.y = sin(lat);
			vertexData[index].position.z = cos(lat) * sin(lon);
			vertexData[index].position.w = 1.0f;
			vertexData[index].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData[index].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			vertexData[index].normal.x = vertexData[index].position.x;
			vertexData[index].normal.y = vertexData[index].position.y;
			vertexData[index].normal.z = vertexData[index].position.z;
		}
	}

	const uint32_t kNumIndices = kSubdivision * kSubdivision * 6;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = CreateBufferResource(device, sizeof(uint32_t) * kNumIndices);

	// インデックスバッファビューを作成する
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * kNumIndices;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// インデックスリソースにデータを書き込む
	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	// インデックスデータを作成
	uint32_t index = 0;
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t topLeft = latIndex * (kSubdivision + 1) + lonIndex;
			uint32_t bottomLeft = (latIndex + 1) * (kSubdivision + 1) + lonIndex;

			// 三角形1
			indexData[index++] = topLeft;
			indexData[index++] = bottomLeft;
			indexData[index++] = topLeft + 1;

			// 三角形2
			indexData[index++] = topLeft + 1;
			indexData[index++] = bottomLeft;
			indexData[index++] = bottomLeft + 1;
		}
	}*/

	// モデル読み込み
	ModelData modelData = LoadObjFile("resources", "plane.obj");
	// 頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = dxCommon->CreateBufferResource(sizeof(Sprite::VertexData) * modelData.vertices.size());
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(Sprite::VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(Sprite::VertexData);

	// 頂点リソースにデータを書き込む
	Sprite::VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(Sprite::VertexData)* modelData.vertices.size());

	// 2枚目のTextureを読んで転送する
	DirectX::ScratchImage mipImages2 = dxCommon->LoadTexture(modelData.material.textureFilePath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource2 = dxCommon->CreateTextureResource(dxCommon->GetDevice(), metadata2);
	dxCommon->UploadTextureData(textureResource2.Get(), mipImages2);

	// Textureを読んで転送する
	DirectX::ScratchImage mipImages = dxCommon->LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = dxCommon->CreateTextureResource(dxCommon->GetDevice(), metadata);
	dxCommon->UploadTextureData(textureResource.Get(), mipImages);

	// metaDataを基にSRVの作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);


	// SRV作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon->GetSRVCPUDescriptorHandle(1);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dxCommon->GetSRVGPUDescriptorHandle(1);

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetSRVCPUDescriptorHandle(2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetSRVGPUDescriptorHandle(2);
	// SRVの作成
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);
	
	Sprite::Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Sprite::Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	Sprite::Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	bool useMonsterBall = true;




	// 出力ウィンドウへの文字出力
	//Log("Hello,DirectX!\n");

	// ウィンドウのxボタンが押されるまでループ
	while (true) {

		// Windowにメッセージが来てたら最優先で処理させる
		if (winApp->ProcessMessage()) {
			// ゲームループを抜ける
			break;;
		}
		else {
			
			input->Update();
			if (input->TriggerKey(DIK_0)){
				OutputDebugStringA("Hit 0\n");
			}
			
		    ImGui_ImplDX12_NewFrame();
		    ImGui_ImplWin32_NewFrame();
		    ImGui::NewFrame();

			// ゲーム処理
			wvpData->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrix = MakePrespectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
			Matrix4x4 worldViewProjectionMatrix = Multiply(wvpData->World, Multiply(viewMatrix, projectionMatrix));
			wvpData->WVP = worldViewProjectionMatrix;

			sprite->Update();

			// 開発用UIの処理
			//ImGui::ShowDemoWindow();

			Vector3 lightDirection = light.direction;

			ImGui::Begin("Setting");
			static ImVec4 triangleColor = ImVec4(color.x, color.y, color.z, color.w); // 初期値は白色
			static ImVec4 lightColor = ImVec4(light.color.x, light.color.y, light.color.z, light.color.w);

			ImGui::DragFloat3("CameraTranslation", &cameraTransform.translate.x, 0.01f);
			ImGui::SliderAngle("CameraRotateX", &cameraTransform.rotate.x);
			ImGui::SliderAngle("CameraRotateY", &cameraTransform.rotate.y);
			ImGui::SliderAngle("CameraRotateZ", &cameraTransform.rotate.z);

			ImGui::SliderAngle("SphereRotateX", &transform.rotate.x);
			ImGui::SliderAngle("SphereRotateY", &transform.rotate.y);
			ImGui::SliderAngle("SphereRotateZ", &transform.rotate.z);

			ImGui::Checkbox("useMonsterBall", &useMonsterBall);
			ImGui::Checkbox("Enable Lighting", reinterpret_cast<bool*>(&materialData->enableLighting));
				
			ImGui::ColorEdit3("Color", (float*)&triangleColor); // 色を編集
			ImGui::ColorEdit3("LightColor", (float*)&lightColor);
			ImGui::SliderFloat3("LightDirectional",(float*) &lightDirection,-1.0f,1.0f);
			ImGui::DragFloat("Intensity", &light.intensity, 0.01f);

			ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScele", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRorate", &uvTransformSprite.rotate.z);

			ImGui::End();

			color = { triangleColor.x, triangleColor.y, triangleColor.z, 1.0f };
			materialData->color = color;

			light.color = { lightColor.x,lightColor.y,lightColor.z,1.0f };
			light.direction = lightDirection;
			directionalLightData->color = light.color;
			directionalLightData->direction = light.direction;
			directionalLightData->intensity = light.intensity;

			Matrix4x4 uvTransformedMatrix = MakeScaleMatrix(uvTransformSprite.scale);
			uvTransformedMatrix = Multiply(uvTransformedMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
			uvTransformedMatrix = Multiply(uvTransformedMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
			//materialDataSprite->uvTransform = uvTransformedMatrix;

			// ImGuiの内部コマンドを発生する
			ImGui::Render();


			dxCommon->PreDraw();

			// Spriteの描画準備
			spriteCommon->SettingCommonDraw();
			// RotSignatureを設定。PSOに設定しているけどベット設定が必要
			//dxCommon->GetCommandlist()->SetGraphicsRootSignature(rootSignature.Get());
			//dxCommon->GetCommandlist()->SetPipelineState(graphicsPipelineState.Get());
			dxCommon->GetCommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);
			//commandList->IASetIndexBuffer(&indexBufferView);


			// 形状を設定
			//dxCommon->GetCommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			// マテリアルCBufferの場所を設定
			dxCommon->GetCommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			// 平行光源のCBufferの場所を設定
			dxCommon->GetCommandlist()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
			// wvp用のCBufferの場所を設定
			dxCommon->GetCommandlist()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
			// SRVのDescriptorTableの先頭を設定
			dxCommon->GetCommandlist()->SetGraphicsRootDescriptorTable(2, useMonsterBall ? textureSrvHandleGPU2 : textureSrvHandleGPU);
			// 描画
			//commandList->DrawIndexedInstanced(kNumIndices, 1, 0, 0, 0);
			dxCommon->GetCommandlist()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

			dxCommon->GetCommandlist()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

			// Spriteの描画
			sprite->Draw();

			// 実際のcommandListのImGuiの描画コマンドを積む
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandlist());


			dxCommon->PostDraw();
		}
	}

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//CloseHandle(fenceEvent);
	delete sprite;
	delete spriteCommon;
	delete input;
	delete dxCommon;

#ifdef _DEBUG

#endif
	winApp->Finalize();
	delete winApp;

	return 0;
}

