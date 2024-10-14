#define NRI_ENABLE_DRAW_PARAMETERS_EMULATION

#include "NRICompatibility.hlsli"
#include "SceneViewerBindlessStructs.h"

NRI_PUSH_CONSTANTS(CullingConstants, Constants, 0);
NRI_RESOURCE(StructuredBuffer<MaterialData>, Materials, t, 0, 0);
NRI_RESOURCE(StructuredBuffer<MeshData>, Meshes, t, 1, 0);
NRI_RESOURCE(StructuredBuffer<InstanceData>, Instances, t, 2, 0);
NRI_RESOURCE(StructuredBuffer<float4x4>, lWMatrixes, t, 3, 0);

NRI_RESOURCE(RWBuffer<uint>, DrawCount, u, 0, 0);
NRI_RESOURCE(RWBuffer<uint>, Commands, u, 1, 0);


groupshared uint s_DrawCount;

#define CTA_SIZE 256

bool Render(uint id) {
    if (id >= MAX_TRANSFORMS) {
        return false;
    }

    if (Instances[id].meshIndex >= Constants.MeshCount) {
        return false;
    }

    return true;
}

float4x4 CreateLWMatrix(uint id) {
    uint parent = Instances[id].parent;
    float4x4 curMat = Instances[id].transform;

    for (uint i = 0; i < MAX_TRANSFORMS && parent < MAX_TRANSFORMS; i++)
    {
        curMat = curMat * Instances[parent].transform;
        parent = Instances[parent].parent;
    }

    return curMat;
}

[numthreads(CTA_SIZE, 1, 1)]
void main(uint threadId : SV_DispatchThreadId)
{
    if (threadId == 0)
        s_DrawCount = 0;

    GroupMemoryBarrierWithGroupSync();

    for (uint instanceIndex = threadId; instanceIndex < Constants.DrawCount; instanceIndex += CTA_SIZE)
    {
        if (!Render(instanceIndex)) {
            continue;
        }
        uint drawIndex = 0;
        InterlockedAdd(s_DrawCount, 1, drawIndex);

        uint meshIndex = Instances[instanceIndex].meshIndex;

        // todo create local to world

        NRI_FILL_DRAW_INDEXED_DESC(Commands, drawIndex,
             Meshes[meshIndex].idxCount,
             1, // TODO: batch draw instances with same mesh into one draw call
             Meshes[meshIndex].idxOffset,
             Meshes[meshIndex].vtxOffset,
             instanceIndex
         );
    }

    GroupMemoryBarrierWithGroupSync();

    if (threadId == 0)
        DrawCount[0] = s_DrawCount;
}


