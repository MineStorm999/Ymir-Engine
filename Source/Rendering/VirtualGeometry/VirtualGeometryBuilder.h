#pragma once

#include <string>

#include "../../Assets/AssetManager.h"

class VirtualGeometryBuilder {
public:
	VirtualGeometryBuilder() {};

	static void BuildVG(std::string inPath, std::string name, bool forceUpdate);
};