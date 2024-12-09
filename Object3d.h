#pragma once
#include"Object3d.h"
#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
#include "MathFunctions.h"

using namespace MathFunctions;

// 3Dオブジェクト
class Object3d {
public:
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct MaterialData {
		std::string textureFilePath;
	};
	struct ModelData {
		std::vector<VertexData> vertices:
		MaterialData material;
	};
public:
	// 初期化
	void Initialize();

	void LoadObjFile(const std::string& directoryPath, const std::string& filename);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
};