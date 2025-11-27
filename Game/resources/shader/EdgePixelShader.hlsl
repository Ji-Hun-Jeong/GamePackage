#include "Common.hlsli"

Texture2D Image : register(t0);
SamplerState Sampler : register(s0);

cbuffer ColorConst : register(b0)
{
    float3 Color;
    float Alpha;
}

cbuffer EdgeConst : register(b1)
{
    float3 EdgeColor;
    uint EdgeRange; // 픽셀 단위 테두리 범위
    
    float Width; // 텍스처 너비
    float Height; // 텍스처 높이
    uint UseImage;
    uint Dummy;
}

float4 main(PixelShaderInput InInput) : SV_TARGET
{
    float2 uv = InInput.UV;

    // 픽셀 단위 테두리를 UV 기준으로 변환
    float edgeThresholdX = (float) EdgeRange / Width;
    float edgeThresholdY = (float) EdgeRange / Height;

    bool isEdge =
        uv.x < edgeThresholdX || uv.x > (1.0 - edgeThresholdX) ||
        uv.y < edgeThresholdY || uv.y > (1.0 - edgeThresholdY);

    if (isEdge)
        return float4(EdgeColor, 1.0); // 설정된 테두리 색상

    if (UseImage)
        return Image.Sample(Sampler, uv); // 원래 텍스처 색
    else
        return float4(Color, Alpha);
}