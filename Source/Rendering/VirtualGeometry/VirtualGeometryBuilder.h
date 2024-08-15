#pragma once

#include <string>

#include "../../Assets/AssetManager.h"

class VirtualGeometryBuilder {
public:
	VirtualGeometryBuilder() {};

	static VModel* BuildVGImpl(std::vector<unsigned int> indices, std::vector<utils::Vertex> vertices, std::string name, bool forceUpdate);
	static std::vector<VModel*> BuildVG(std::string inPath, std::string name, bool forceUpdate, bool buildAsSingleMesh);
};