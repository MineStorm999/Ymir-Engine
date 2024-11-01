#define NRI_ENABLE_DRAW_PARAMETERS_EMULATION

#include "NRICompatibility.hlsli"
#include "SceneViewerBindlessStructs.h"

NRI_RESOURCE(StructuredBuffer<uint>, Indices, t, 0, 0);
NRI_RESOURCE(StructuredBuffer<float3>, Vertices, t, 1, 0);
NRI_RESOURCE(StructuredBuffer<OdCollider>, Collider, t, 2, 0);
NRI_RESOURCE(StructuredBuffer<OdCluster>, Cluster, t, 3, 0);
NRI_RESOURCE(StructuredBuffer<OdRigidBody>, RigidBodies, t, 4, 0);

NRI_RESOURCE(RWBuffer<PositionRB>, PositionRB, u, 0, 0);
NRI_RESOURCE(RWBuffer<RigidBodyRB>, RigidBodyRB, u, 0, 0);
NRI_RESOURCE(RWBuffer<CollisionEvent>, CollisionEventRB, u, 0, 0);

#define THREAD_COUNT 512

[numthreads(THREAD_COUNT, 1, 1)]
void main(uint threadId : SV_DispatchThreadId)
{
    if (threadId == 0)
        s_DrawCount = 0;

    GroupMemoryBarrierWithGroupSync();


    GroupMemoryBarrierWithGroupSync();

    if (threadId == 0)
        DrawCount[0] = s_DrawCount;
}


