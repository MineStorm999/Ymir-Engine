#include "NRICompatibility.hlsli"

// todo

#define CTA_SIZE 512

[numthreads(CTA_SIZE, 1, 1)]
void main(uint threadId : SV_DispatchThreadId)
{
    return;
}