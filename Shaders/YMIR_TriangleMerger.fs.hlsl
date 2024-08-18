// © 2021 NVIDIA Corporation

#include "NRICompatibility.hlsli"



struct outputVS
{
    float4 position : SV_Position;
    float2 texCoord : UV;
};

float4 main(in outputVS input) : SV_Target
{
    float4 output;
    return output;
}
