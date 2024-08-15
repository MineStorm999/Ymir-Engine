#pragma once


#include "NRIFramework.h"


struct ModelDesc{
	uint32_t meshletOffset;
	uint32_t meshletCount;

	uint32_t rootMeshlet;
};

struct InstanceDesc{
	uint32_t modelID;
	uint32_t materialID;
	
	float4x4 transform;
};

struct MeshletDesc {
	// culling
	float4 boundingSphere;

	float3 cone_axis;
	float cone_cutoff;

	uint32_t childOff;
	uint32_t childCount;

	uint32_t vertTriCount; // 16bit vertex, 8bit triCount(packed in 8bit), 8bit trieCount (round to 32bit for loading)  

	uint32_t modelID;

	// only for loading
	uint32_t clusterOffset;
	uint32_t clusterLenght;
};

struct MeshletLoadDesc {
	uint32_t offset;
	//uint32_t vertTriCount; // 16bit vertex, 8bit triCount(packed in 8bit), 8bit trieCount (round to 32bit for loading)   

	uint32_t modelID;
	uint32_t clusterOffset;
	uint32_t clusterLenght;
};

struct RenderCmd {
	uint32_t offset;
	uint32_t vertTriCount; // 16bit vertex, 8bit triCount(packed in 8bit), 8bit trieCount (round to 32bit for loading)  

	uint32_t instanceID;
};

struct Vertex {
	float3 pos;
	float4 norm; //    0-2 = normal, 3 = texcoord0
	float4 tangent; // 0-2 = tangent, 3 = texcoord1
};
