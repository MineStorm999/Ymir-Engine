#define NRI_ENABLE_DRAW_PARAMETERS_EMULATION

#include "NRICompatibility.hlsli"
#include "SceneViewerBindlessStructs.h"

NRI_PUSH_CONSTANTS(CullingConstants, Constants, 0);
NRI_RESOURCE(StructuredBuffer<MaterialData>, Materials, t, 0, 0);
NRI_RESOURCE(StructuredBuffer<MeshData>, Meshes, t, 1, 0);
NRI_RESOURCE(StructuredBuffer<InstanceData>, Instances, t, 2, 0);
NRI_RESOURCE(StructuredBuffer<BatchDesc>, Batches, t, 3, 0);
NRI_RESOURCE(RWBuffer<uint>, DrawCount, u, 0, 0);
NRI_RESOURCE(RWBuffer<uint>, Commands, u, 1, 0);


groupshared uint s_DrawCount;

#define CTA_SIZE 256

[numthreads(CTA_SIZE, 1, 1)]
void main(uint threadId : SV_DispatchThreadId)
{
    if (threadId == 0)
        s_DrawCount = 0;

    GroupMemoryBarrierWithGroupSync();

    for (uint batchIndex = threadId; batchIndex < Constants.DrawCount; batchIndex += CTA_SIZE)
    {
        uint drawIndex = 0;
        InterlockedAdd(s_DrawCount, 1, drawIndex);

        uint meshIndex = Instances[Batches[batchIndex].offset].meshIndex;

        NRI_FILL_DRAW_INDEXED_DESC(Commands, drawIndex,
            Meshes[meshIndex].idxCount,
            Batches[batchIndex].count, // TODO: batch draw instances with same mesh into one draw call
            Meshes[meshIndex].idxOffset,
            Meshes[meshIndex].vtxOffset,
            Batches[batchIndex].offset
        );
    }

    GroupMemoryBarrierWithGroupSync();

    if (threadId == 0)
        DrawCount[0] = s_DrawCount;
}