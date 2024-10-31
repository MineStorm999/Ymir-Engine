

#ifndef THIS
#define THIS
#include "../External/NRIFramework/External/NRI/Include/NRICompatibility.hlsli"

#define MAX_TRANSFORMS 3000000
#define INVALID 0xffffffff

struct CullingConstants
{
	float4 Frustum;
	uint32_t DrawCount;
	uint32_t MeshCount;
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
    uint32_t entityID;
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


// Od Physics
#define CUBE_COLLIDER INVALID - 1
#define SPHERE_COLLIDER INVALID - 2

#define MAX_RIGID_BODIES MAX_TRANSFORMS
#define MAX_READBACKS 50000

struct OdCollider {
    float4 boundingSphere;

    uint32_t clusterOffset;
    uint32_t clusterCount;
};

struct OdCluster {
    float4 boundingSphere;

    uint32_t indexOffset;
    uint32_t indexCount;

    uint32_t vertexOffset;
    uint32_t vertexCount;
};

struct OdRigidBody {
    float4 velocity;
    float4 angularVelocity;
};

struct PositionRB {
    uint32_t id;
    uint32_t off0;
    uint32_t off1;
    uint32_t off2;
    float4 position;
    float4 rotation;
};

struct RigidBodyRB {
    uint32_t id;
    uint32_t off0;
    uint32_t off1;
    uint32_t off2;
    float4 velocity;
    float4 rotVelocity;
};

struct CollisionEvent {
    uint32_t first;
    uint32_t second;
    uint32_t off0;
    uint32_t off1;
    float4 velocity_Magnitude;
    float4 worldPosition;
};

// END      END             END                 END
#endif // !THIS