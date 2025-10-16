#include "Common.hlsli"

#define Matrix matrix
cbuffer ConstBuffer : register(b0)
{
    Matrix Model;
};

ColorPixelShaderInput main(ColorVertexShaderInput InInput)
{
    ColorPixelShaderInput Output;
    float4 Position = float4(InInput.Position, 1.0f);
    Position = mul(Position, Model);
    
    Output.Position = Position;
    Output.Color = InInput.Color;
    return Output;
}