#include "Common.hlsli"

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

float4 main(PixelShaderInput InInput) : SV_TARGET
{
    float2 uv = InInput.UV;

    // 가장자리 기준 (5% 이내)
    float edgeThreshold = 0.01;

    bool isEdge =
        uv.x < edgeThreshold || uv.x > (1.0 - edgeThreshold) ||
        uv.y < edgeThreshold || uv.y > (1.0 - edgeThreshold);

    if (isEdge)
        return float4(1.0, 0.0, 0.0, 1.0); // 빨간색

    return Image.Sample(Sampler, uv); // 원래 텍스처 색
}