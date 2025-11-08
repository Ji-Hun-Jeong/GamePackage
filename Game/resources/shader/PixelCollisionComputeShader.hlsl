#include "Common.hlsli"

struct PixelCollider
{
    float2 Position;
    float2 Scale;
    uint bCollision;
};

RWStructuredBuffer<PixelCollider> PixelColliders : register(u0);

// float2 ConvertWorldPositionToImageUV(float3 _Position, uint2 _ImageScale)
// {
//     _Position.xy *= float2(ScreenWidth, ScreenHeight) / float2(_ImageScale);
//     
//     float2 UV = clamp((_Position.xy * float2(1.0f, -1.0f) + 1.0f) / 2.0f, 0.0f, 1.0f);
//     
//     return UV;
// }
// void ConvertImageUVToWorldPosition(inout float3 _Position, float2 _UV, uint2 _ImageScale)
// {
//     _Position = float3((_UV * 2.0f - 1.0f) * float2(1.0f, -1.0f), _Position.z);
//     _Position.xy *= float2(_ImageScale) / float2(ScreenWidth, ScreenHeight);
// }
// 
// bool Collision(inout float3 _Position, float2 _UV, uint2 _ImageScale, int2 _OffsetPixel, Texture2D _Image)
// {
//     uint2 OriginPosition = _UV * _ImageScale;
//     
//     for (int i = -_OffsetPixel.x; i < _OffsetPixel.x; ++i)
//     {
//         float2 Position = OriginPosition + int2(i, 0);
//         float2 UV = Position / float2(_ImageScale);
//         UV = clamp(UV, 0.0f, 1.0f);
//         float4 PixelColor = _Image.SampleLevel(PointSampler, UV, 0);
//         
//         if (PixelColor.r != 1.0f || PixelColor.g != 1.0f || PixelColor.b != 1.0f)
//         {
//             ConvertImageUVToWorldPosition(_Position, UV, _ImageScale);
//             return true;
//         }
//            
//     }
//     
//     for (int j = -_OffsetPixel.y; j < _OffsetPixel.y; ++j)
//     {
//         float2 Position = OriginPosition + int2(0, j);
//         float2 UV = Position / float2(_ImageScale);
//         UV = clamp(UV, 0.0f, 1.0f);
//         float4 PixelColor = _Image.SampleLevel(PointSampler, UV, 0);
//         
//         if (PixelColor.r != 1.0f || PixelColor.g != 1.0f || PixelColor.b != 1.0f)
//         {
//             ConvertImageUVToWorldPosition(_Position, UV, _ImageScale);
//             return true;
//         }
//     }
//     
//     return false;
// }
[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // WorldPosition -> UV
    // CollisionCheck
    PixelColliders[DTid.x].Position.x = 523.0f;
    PixelColliders[DTid.x].bCollision = 1;
}