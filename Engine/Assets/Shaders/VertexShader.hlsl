
cbuffer Transform : register(b0)
{
    float4x4 MVP;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput o;
    o.position = mul(MVP, float4(input.position, 1.0f));
    o.normal = input.normal;
    o.uv = input.uv;
    return o;
}
