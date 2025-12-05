#include "Common.hlsli"
Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

cbuffer SpriteData : register(b0)
{
    float3 Color;
    float Alpha;
    
    uint bUseImage;
    uint3 Dummy;
}

float4 main(PixelShaderInput InInput) : SV_TARGET
{
    float4 FinalColor = float4(Color, Alpha);
    if (bUseImage)
        FinalColor *= Image.Sample(Sampler, InInput.UV);
    return FinalColor;
}