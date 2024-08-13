struct Model{
	uint32_t meshletOffset;
	uint32_t meshletCount;
};

struct Instance{
	uint32_t modelID;
	uint32_t materialID;

	uint32_t rootMeshlet;
	
	float4x4 mat;
};

struct MeshletDesc {
	float4 boundingBox;

	uint32_t childOff;
	uint32_t childCount;

	uint32_t vertTriCount; // 16bit vertex, 16bit index 

	uint32_t modelID;
};

struct MeshletLoadDesc {
	uint32_t offset;
	uint32_t vertTriCount; // 16bit vertex, 16bit index 

	uint32_t modelID;
	uint32_t clusterOffset;
	uint32_t clusterLenght;
};

struct RenderCmd {
	uint32_t offset;
	uint32_t vertTriCount; // 16bit vertex, 16bit index 

	uint32_t instanceID;
};

struct Vertex {
	float3 pos;
	float4 norm; //    0-2 = normal, 3 = texcoord0
	float4 tangent; // 0-2 = tangent, 3 = texcoord1
};