#pragma once
#include <memory>
#include <unordered_map>

#include "../../Assets/AssetManager.h"
#include "MeshletStructs.h"

using ModelMap = std::vector<FILE*>;

class VirtualGeometryStreamer {
public:
	void Init();

	void Load(uint32_t modelID, void* buffer, uint32_t clusterOffset, uint32_t clusterlenght);

	void UnInit();

public:
	std::vector<ModelDesc> modelDescs;
	std::vector<MeshletDesc> meshletDescs;
	
private:
	ModelMap models;
};