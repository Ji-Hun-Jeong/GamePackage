struct PSInput
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};

float4 main(PSInput InInput) : SV_TARGET
{
    return float4(InInput.Color, 1.0f);
}