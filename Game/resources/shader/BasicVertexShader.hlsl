#include "Common.hlsli"

#define Matrix matrix

StructuredBuffer<Matrix> Models : register(t0);

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

PixelShaderInput main(VertexShaderInput InInput, uint InInstanceId : SV_InstanceID)
{
    PixelShaderInput Output;
    float4 Position = float4(InInput.Position, 1.0f);
    Position = mul(Position, Model);
    //Position = mul(Position, Models[InInstanceId]);
    Position = mul(Position, ViewProj);
    
    Output.Position = Position;
    Output.UV = InInput.UV;
    return Output;
}