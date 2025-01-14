#pragma once

#include "MathLib/ml.h"

struct BrixelizerGIInitDesc {
	uint2 screenSize;
	float giScale;
	uint32_t cascades;
	float staticCascadeRes;
	float dynamicCascadeRes;

	float voxelSize = 0.2f;
	float voxelCascadeRatio = 2.f; 
};

class BrixelizerGIInterFace {
public:
	BrixelizerGIInterFace(const BrixelizerGIInitDesc& desc);
};