#pragma once

#include "Assets/assetTypes.h"

struct MeshImportSettings {
	bool autoLOD{false};
	bool singleModel{false};
	bool compress{true};
	bool optimize{true};

	int lodCount;
};

class MeshLoader
{
public:
	static std::vector<AModel*> ImportModel(std::string path, std::string savePath, std::string name, MeshImportSettings settings);
};