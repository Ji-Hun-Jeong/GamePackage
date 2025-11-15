#include "Common.hlsli"

#define Matrix matrix

cbuffer ConstBuffer : register(b0)
{
    Matrix Model;
};

cbuffer GlobalConstBuffer : register(b1)
{
    Matrix ViewProj;
    uint ScreenWidth;
    uint ScreenHeight;
    uint2 Dummy;
};

ColorPixelShaderInput main(ColorVertexShaderInput InInput)
{
    ColorPixelShaderInput Output;
    float4 Position = float4(InInput.Position, 1.0f);
    Position = mul(Position, Model);
    Position = mul(Position, ViewProj);
    
    Output.Position = Position;
    Output.Color = InInput.Color;
    return Output;
}