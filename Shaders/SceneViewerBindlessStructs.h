

#ifndef THIS
#define THIS
#include "../External/NRIFramework/External/NRI/Include/NRICompatibility.hlsli"


#define MAX_TRANSFORMS 3000000

struct CullingConstants
{
	float4 Frustum;
	uint32_t DrawCount;
	uint32_t BatchCount;
	uint32_t EnableCulling;
	uint32_t ScreenWidth;
	uint32_t ScreenHeight;
};

struct MaterialData
{
    float4 baseColorAndMetallic;
    float4 emissiveColorAndRoughness;
    uint32_t baseColorTexIndex;
    uint32_t roughnessMetalnessTexIndex;
    uint32_t normalTexIndex;
    uint32_t emissiveTexIndex;
};

struct MeshData
{
    uint32_t vtxOffset;
    uint32_t vtxCount;
    uint32_t idxOffset;
    uint32_t idxCount;
};

struct InstanceData
{
    uint32_t meshIndex;
    uint32_t materialIndex;

    uint32_t parent;
    uint32_t off0;
    float4x4 transform;

};

struct BatchDesc {
    uint32_t offset;
    uint32_t count;
};

NRI_RESOURCE(cbuffer, GlobalConstants, b, 0, 0 )
{
    float4x4 gWorldToClip;
    float3 gCameraPos;
    uint32_t batchCount;
};
#endif // !THIS