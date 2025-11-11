#include "Common.hlsli"

struct PixelCollider
{
    float2 Position;
    float2 Scale;
    uint bCollision[6];
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
    if (NumOfColliders <= DTid.x)
        return;
    
    float2 Position = PixelColliders[DTid.x].Position;
    float2 Scale = PixelColliders[DTid.x].Scale;
    if (Collision(Position, int2(0, Scale.y / 2.0f), FloorMap))
        PixelColliders[DTid.x].bCollision[0] = 1;
    
    if (Collision(Position, int2(Scale.x, 0), LadderMap))
        PixelColliders[DTid.x].bCollision[1] = 1;
    
    if (Collision(Position, int2(Scale.x, 0), RopeMap))
        PixelColliders[DTid.x].bCollision[2] = 1;
    
    if (Collision(Position, int2(Scale.x, 0), WallMap))
        PixelColliders[DTid.x].bCollision[3] = 1;
    
    if (Collision(Position, int2(Scale.x, 0), MonsterWallMap))
        PixelColliders[DTid.x].bCollision[4] = 1;
    
    //uint ImageWidth = 0;
    //uint ImageHeight = 0;
    //LadderMap.GetDimensions(ImageWidth, ImageHeight);
    //
    //
    //if (DTid.x == 0)
    //{
    //    float3 FinalColor = 0.0f;
    //    for (int i = 0; i < 30; ++i)
    //    {
    //        for (int j = 0; j < 30; ++j)
    //        {
    //            float2 UV = ConvertToUVPosition(Position + float2(i, j), uint2(ImageWidth, ImageHeight));
    //            FinalColor += LadderMap.SampleLevel(PointSampler, UV, 0.0f).rgb;
    //        }
    //    }
    //    
    //    PixelColliders[DTid.x].Position.x = FinalColor.r;
    //    PixelColliders[DTid.x].Position.y = FinalColor.g;
    //}

}