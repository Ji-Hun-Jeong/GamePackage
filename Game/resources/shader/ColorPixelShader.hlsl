#include "Common.hlsli"
Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

cbuffer ColorConstBuffer : register(b0)
{
    float3 Color;
    float Transparency;
}

float4 main(PixelShaderInput InInput) : SV_TARGET
{
    return float4(Color, Transparency);
}