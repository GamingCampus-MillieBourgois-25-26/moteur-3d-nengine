Texture2D diffuseTex : register(t0);
SamplerState samp : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_Target
{
    /*float3 lightDir = normalize(float3(0.5, 1.0, -0.3));
    float NdotL = saturate(dot(normalize(input.normal), lightDir));
    float3 baseColor = float3(1.0, 0.2, 0.2);
    float3 color = baseColor * (0.2 + 0.8 * NdotL);
    return float4(color, 1.0);*/
    
        // Direction de la lumière
    float3 lightDir = normalize(float3(0.5, 1.0, -0.3));

    // Intensité de la lumière
    float NdotL = saturate(dot(normalize(input.normal), lightDir));

    // Lecture de la texture diffuse
    float4 texColor = diffuseTex.Sample(samp, input.uv);

    // Éclairage simple (Lambert)
    float3 litColor = texColor.rgb * (0.2 + 0.8 * NdotL);

    return float4(litColor, texColor.a);
}