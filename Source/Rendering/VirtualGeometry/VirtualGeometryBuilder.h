#pragma once

#include <string>

#include "../../Assets/AssetManager.h"

class VirtualGeometryBuilder {
public:
	VirtualGeometryBuilder() {};

	static AssetBase* BuildVG(std::string inPath, bool forceUpdate);
};