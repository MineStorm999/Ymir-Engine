#pragma once

#include "Assets/assetTypes.h"

struct MeshImportSettings {
	bool autoLOD{false};
	bool singleModel{false};
	bool createFolder{ true };
	bool compress{true};
	bool optimize{true};

	int lodCount;
};

class Importer
{
public:
	static void ImportModel(std::string path, std::string savePath, std::string name, MeshImportSettings settings);

	static std::vector<unsigned int> CreateLOD(float siplificationAmmount, const unsigned int* originalIndices, uint32_t indexCount, const void* originalVerices, uint32_t vertexCount);

	// only temporary
	static AssetID ImportTexture(std::string path, std::string savePath, std::string name);
	// todo
	static AssetID ImportTexture(utils::Texture texture, std::string savePath);
	// todo
	static AssetID ImportMaterial(utils::Material, std::string savePath, std::string name);
};