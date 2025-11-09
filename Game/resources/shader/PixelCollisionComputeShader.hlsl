#include "Common.hlsli"

struct PixelCollider
{
    float2 Position;
    uint bCollision0;
    uint bCollision1;
    uint bCollision2;
    uint bCollision3;
    uint bCollision4;
    uint bCollision5;
};

RWStructuredBuffer<PixelCollider> PixelColliders : register(u0);

Texture2D FloorMap : register(t0);
Texture2D LadderMap : register(t1);
Texture2D RopeMap : register(t2);
Texture2D WallMap : register(t3);
Texture2D MonsterWallMap : register(t4);
Texture2D OtherMap : register(t5);

SamplerState PointSampler : register(s0);

cbuffer ConstBuffer : register(b0)
{
    uint ScreenWidth;
    uint ScreenHeight;
    uint NumOfColliders;
    uint Dummy;
}

float2 ConvertToUVPosition(float2 InPosition, uint2 InStandardScale)
{
    float2 UVPosition = InPosition;
    UVPosition.x = (UVPosition.x + InStandardScale.x / 2.0f) / InStandardScale.x;
    UVPosition.y = (-UVPosition.y + InStandardScale.y / 2.0f) / InStandardScale.y;
    return UVPosition;
}
bool IsNotBlack(float4 InColor)
{
    return InColor.r != 0.0f || InColor.g != 0.0f || InColor.b != 0.0f;
}
bool Collision(float2 InPosition, int2 InOffset, Texture2D InTexture)
{
    uint ImageWidth = 0;
    uint ImageHeight = 0;
    InTexture.GetDimensions(ImageWidth, ImageHeight);
    
    for (int i = -InOffset.x; i < InOffset.x; ++i)
    {
        float2 UVPosition = saturate(ConvertToUVPosition(InPosition + float2(i, 0), uint2(ImageWidth, ImageHeight)));
        float4 Color = InTexture.SampleLevel(PointSampler, UVPosition, 0);
        if (IsNotBlack(Color))
            return true;
    }
    for (int j = -InOffset.y; j < InOffset.y; ++j)
    {
        float2 UVPosition = saturate(ConvertToUVPosition(InPosition + float2(0, j), uint2(ImageWidth, ImageHeight)));
        float4 Color = InTexture.SampleLevel(PointSampler, UVPosition, 0);
        if (IsNotBlack(Color))
            return true;
    }
    return false;
}

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if(NumOfColliders <= DTid.x)
        return;
    
    float2 Position = PixelColliders[DTid.x].Position;
    if (Collision(Position, int2(0, 10), FloorMap))
        PixelColliders[DTid.x].bCollision0 = true;
    
}