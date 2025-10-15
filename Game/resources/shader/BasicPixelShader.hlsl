#include "Common.hlsli"

float4 main(PSInput InInput) : SV_TARGET
{
    return float4(InInput.Color, 1.0f);
}