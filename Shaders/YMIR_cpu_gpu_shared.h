#pragma once

//#define ReadBacks 1000000

#define RENDERCMDS 15000000
#define READBACKS 10000000
#define MAXMESHINSTANCES 1000000

NRI_RESOURCE(cbuffer, GlobalConstants, b, 0, 0)
{
    float4x4 gWorldToClip;
    float3 gCameraPos;

    uint32_t instanceCountOld;
    uint32_t instanceCountNew;
};