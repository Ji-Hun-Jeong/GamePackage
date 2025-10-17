#include "Common.hlsli"
Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

float4 main(PixelShaderInput InInput) : SV_TARGET
{
    return Image.Sample(Sampler, InInput.UV);
}