// © 2021 NVIDIA Corporation

#define NRI_ENABLE_DRAW_PARAMETERS_EMULATION

#include "NRICompatibility.hlsli"
#include "SceneViewerBindlessStructs.h"

NRI_ENABLE_DRAW_PARAMETERS;

NRI_RESOURCE(StructuredBuffer<InstanceData>, Instances, t, 2, 0);

struct Input
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Tangent : TANGENT;
};

struct Attributes
{
    float4 Position : SV_Position;
    float4 Normal : TEXCOORD0; //.w = TexCoord.x
    float4 View : TEXCOORD1; //.w = TexCoord.y
    float4 Tangent : TEXCOORD2;
    nointerpolation uint DrawParameters : ATTRIBUTES;
};

float4 GetWorldPos(uint id, float4 pos) {
    /*uint parent = Instances[id].parent;
    float4 curPos = mul(Instances[id].transform, pos);

    for (uint i = 0; i < MAX_TRANSFORMS && parent < MAX_TRANSFORMS; i++)
    {
        curPos = mul(Instances[parent].transform, curPos);
        parent = Instances[parent].parent;
    }
    
    return curPos;*/

    return mul(Instances[id].transform, pos);
}

Attributes main( in Input input, NRI_DECLARE_DRAW_PARAMETERS )
{
    Attributes output = (Attributes)0;

#ifndef NRI_DXBC
    float3 N = input.Normal * 2.0 - 1.0;
    float4 T = input.Tangent * 2.0 - 1.0;
    float3 V = gCameraPos - input.Position;

    

    output.Position = GetWorldPos(NRI_INSTANCE_ID_OFFSET, float4(input.Position, 1.0f));
    output.Position =  mul(gWorldToClip, output.Position);
    output.Normal = float4( N, input.TexCoord.x );
    output.View = float4( V, input.TexCoord.y );
    output.Tangent = T;
    output.DrawParameters = NRI_INSTANCE_ID_OFFSET;
#endif

    return output;
}
