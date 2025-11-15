#include "Common.hlsli"

float4 main(ColorPixelShaderInput InInput) : SV_TARGET
{
    return float4(InInput.Color, 1.0f);
}