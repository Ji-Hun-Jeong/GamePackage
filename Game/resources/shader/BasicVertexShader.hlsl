#include "Common.hlsli"

#define Matrix matrix
cbuffer ImageResizeConstBuffer : register(b0)
{
    Matrix SubModel;
}

cbuffer ConstBuffer : register(b1)
{
    Matrix Model;
};

cbuffer GlobalConstBuffer : register(b2)
{
    Matrix ViewProj;
    uint ScreenWidth;
    uint ScreenHeight;
    uint2 Dummy;
};

PixelShaderInput main(VertexShaderInput InInput)
{
    PixelShaderInput Output;
    float4 Position = float4(InInput.Position, 1.0f);
    Position = mul(Position, SubModel);
    Position = mul(Position, Model);
    Position = mul(Position, ViewProj);
    
    Output.Position = Position;
    Output.UV = InInput.UV;
    return Output;
}