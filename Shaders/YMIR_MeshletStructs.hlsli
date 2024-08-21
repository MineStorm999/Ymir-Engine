#pragma once

struct ModelDesc{
	uint32_t meshletOffset;
	uint32_t meshletCount;

	uint32_t rootMeshlet;
};

struct InstanceDesc{
	uint32_t modelID;
	uint32_t materialID;
	
	float4x4 transform;

	uint32_t realID;
};

struct MeshletDesc {
	// culling
	float4 boundingSphere;

	float3 cone_axis;
	float cone_cutoff;

	uint32_t childOff;
	uint32_t childCount;

	uint32_t vertTriCount; // 16bit vertex, 16bit triCount(packed in 8bit)

	uint32_t modelID;

	// only for loading
	uint32_t clusterOffset;
	uint32_t clusterLenght;
};

struct MeshletLoadDesc {
	uint32_t clusterID;
	//uint32_t vertTriCount; // 16bit vertex, 16bit triCount(packed in 8bit)

	uint32_t modelID;
	uint32_t clusterOffset;
	uint32_t clusterLenght;
};

struct RenderCmd {
	uint32_t realClusterID;
	uint32_t offset;
	uint32_t vertTriCount;  // 16bit vertex, 16bit triCount(packed in 8bit)

	uint32_t instanceID;
};

struct Vertex {
	float3 pos;
	float3 norm;
	float3 tangent; 
	uint32_t uv; // 16bit x, y	// unpack: value / 2^16
};