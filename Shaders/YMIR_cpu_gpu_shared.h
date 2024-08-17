#pragma once

#define ReadBacks 1000000

#define RenderCMDs 1000000
#define ReadBacks 1000000

NRI_RESOURCE(cbuffer, GlobalConstants, b, 0, 0)
{
    float4x4 gWorldToClip;
    float3 gCameraPos;

    uint32_t instanceCountOld;
    uint32_t instanceCountNew;
};