#pragma once

#include <string>

#include "../../Assets/AssetManager.h"

class VirtualGeometryBuilder {
public:
	VirtualGeometryBuilder() {};

	static VModel* BuildVG(std::string inPath, std::string name, bool forceUpdate);
};