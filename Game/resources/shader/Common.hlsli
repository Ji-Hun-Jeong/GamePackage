struct ColorVertexShaderInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};
struct ColorPixelShaderInput
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};
struct VertexShaderInput
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
};
struct PixelShaderInput
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};