#include "NRICompatibility.hlsli"
#include "YMIR_MeshletStructs.hlsli"
#include "YMIR_cpu_gpu_shared.h"
#include "YMIR_ComputeCommon.hlsli"
// todo




#define CTA_SIZE 128



[numthreads(CTA_SIZE, 1, 1)]
void main(uint threadId : SV_DispatchThreadId)
{
    return;
}