// © 2021 NVIDIA Corporation

#include "NRICompatibility.hlsli"

struct outputVS
{
    float4 position : SV_Position;
    float2 texCoord : UV;
};

outputVS main
(
    float2 inPos : POSITION,
    float2 inTexCoord : UV
)
{
    outputVS output;

    output.position.xy = inPos;
    output.position.zw = float2( 0.0, 1.0 );

    output.texCoord = inTexCoord;

    return output;
}
