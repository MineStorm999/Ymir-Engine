#pragma once

#include "NRIFramework.h"

struct Transform {
	float4x4 localMat;
	
	float3 localPos{0,0,0};
	float3 localScale{1,1,1};
	float3 localRot{0,0,0};
};

