#pragma once
#include <NRIFramework.h>
#include <string>
#include "../../Assets/assetTypes.h"



class VirtualGeometryBuilder {
public:
	VirtualGeometryBuilder() {};

	static class VModel* BuildVGImpl(std::vector<unsigned int> indices, std::vector<utils::Vertex> vertices, std::string name, bool forceUpdate);
	static std::vector<class VModel*> BuildVG(std::string inPath, std::string name, bool forceUpdate, bool buildAsSingleMesh);

	static class VModel* LoadCached(std::string name);

	
};