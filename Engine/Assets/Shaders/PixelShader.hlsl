struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_Target
{
    float3 lightDir = normalize(float3(0.5, 1.0, -0.3));
    float NdotL = saturate(dot(normalize(input.normal), lightDir));
    float3 baseColor = float3(1.0, 0.2, 0.2);
    float3 color = baseColor * (0.2 + 0.8 * NdotL);
    return float4(color, 1.0);
}