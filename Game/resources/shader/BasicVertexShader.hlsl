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

PixelShaderInput main(VertexShaderInput InInput)
{
    PixelShaderInput Output;
    float4 Position = float4(InInput.Position, 1.0f);
    Position = mul(Position, SubModel);
    Position = mul(Position, Model);
    
    Output.Position = Position;
    Output.UV = InInput.UV;
    return Output;
}