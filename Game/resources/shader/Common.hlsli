struct VSInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};
struct PSInput
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};