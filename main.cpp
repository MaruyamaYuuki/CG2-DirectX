#include "ResourceObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "D3DResourceLeakChecker.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "Object3dCommon.h"
#include "Object3d.h"
#include "ModelCommon.h"
#include "Model.h"

#pragma comment (lib, "dxcompiler.lib")

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
	// テスクチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon);
	// Textureを読んで転送する
	//TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");


	/*std::vector<Sprite*> sprites;
	for (uint32_t i = 0; i < 5; ++i) {
		Sprite* sprite = new Sprite();

		std::string filePath;
		if (i % 2 == 0) {
			filePath = "resources/uvChecker.png";
		}
		else {
			filePath = "resources/monsterBall.png";
		}
		sprite->Initialize(spriteCommon, filePath);


		// 各スプライトに異なる位置を設定
		Vector2 newPosition = {float(i * 200), 0 };
		sprite->SetPosition(newPosition);

		sprites.push_back(sprite);
	}*/

	Sprite* sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon, "resources/uvChecker.png");

	ModelCommon* modelCommon = nullptr;
	modelCommon = new ModelCommon;
	modelCommon->Initialize(dxCommon);

	Model* model = nullptr;
	model = new Model;
	model->Initialize(modelCommon);

	Object3dCommon* object3dCommon = nullptr;
	object3dCommon = new Object3dCommon;
	object3dCommon->Initialize(dxCommon);

	/*	Object3d* object3d = new Object3d;
	object3d->Initialize(object3dCommon);
	object3d->SetModel(model);*/


	std::vector<Object3d*> object3dList;

	for (int i = 0; i < 2; ++i) { // 5つのオブジェクトを生成
		Object3d* object3d = new Object3d;
		object3d->Initialize(object3dCommon);
		object3d->SetModel(model);
		object3d->SetTranslate(Vector3(float(i * 3), 0.0f, 0.0f));
		object3dList.push_back(object3d);
	}

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




	
	Sprite::Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Sprite::Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	Sprite::Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	bool useMonsterBall = true;

	float roation = sprite_->GetRotation();

	bool isRotation = false;

	float num = 0;

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
			//object3d->Updata();

			Vector3 currentRotate[2];
			for (int i = 0; i < object3dList.size(); ++i) {

				currentRotate[i] = object3dList[i]->GetRotate();
				currentRotate[0].z += 0.05f;
				currentRotate[1].y += 0.05f;

				object3dList[i]->SetRotate(currentRotate[i]);

				// 更新処理
				object3dList[i]->Updata();
			}

			/*for (Sprite* sprite : sprites) {
				sprite->Update();
			}*/
			sprite_->Update();

			// 開発用UIの処理
			//ImGui::ShowDemoWindow();


			ImGui::Begin("Setting");

			ImGui::DragFloat3("CameraTranslation", &cameraTransform.translate.x, 0.01f);
			ImGui::SliderAngle("CameraRotateX", &cameraTransform.rotate.x);
			ImGui::SliderAngle("CameraRotateY", &cameraTransform.rotate.y);
			ImGui::SliderAngle("CameraRotateZ", &cameraTransform.rotate.z);
			/*			ImGui::SliderAngle("SphereRotateX", &transform.rotate.x);
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
			ImGui::SliderAngle("UVRorate", &uvTransformSprite.rotate.z);*/


			ImGui::Checkbox("isRotation", &isRotation);

			ImGui::End();

			Matrix4x4 uvTransformedMatrix = MakeScaleMatrix(uvTransformSprite.scale);
			uvTransformedMatrix = Multiply(uvTransformedMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
			uvTransformedMatrix = Multiply(uvTransformedMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
			//materialDataSprite->uvTransform = uvTransformedMatrix;

			// ImGuiの内部コマンドを発生する
			ImGui::Render();


			dxCommon->PreDraw();

			// 3Dオブジェクトの描画準備
			object3dCommon->SettingCommonDraw();
			//object3d->Draw();
			for (Object3d* object3d : object3dList) {
				object3d->Draw();
			}

			// Spriteの描画準備
			spriteCommon->SettingCommonDraw();
			// Spriteの描画
			/*for (Sprite* sprite : sprites) {
				sprite->Draw();
			}*/
			sprite_->Draw();

			// 実際のcommandListのImGuiの描画コマンドを積む
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandlist());


			dxCommon->PostDraw();
		}
	}

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	for (Object3d* object3d : object3dList) {
		delete object3d;
	}
	object3dList.clear();
	//delete object3d;
	delete object3dCommon;
	delete model;
	delete modelCommon;
	//CloseHandle(fenceEvent);
	/*for (Sprite* sprite : sprites) {
		delete sprite;
	}
	sprites.clear();*/
	delete sprite_;
	delete spriteCommon;
	TextureManager::GetInstance()->Finalize();
	delete input;
	delete dxCommon;

#ifdef _DEBUG

#endif
	winApp->Finalize();
	delete winApp;

	return 0;
}

