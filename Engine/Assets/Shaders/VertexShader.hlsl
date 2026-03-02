
/*cbuffer Transform : register(b0)
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
}*/

// Version ECS

cbuffer Transform : register(b0)
{
    float4x4 world; // applique rotation, scale, translation ECS
    float4x4 view; // applique la camera
    float4x4 proj; // applique la perspective
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

    float4 localPos = float4(input.position, 1.0f);

    // Transformation complète
    float4 worldPos = mul(localPos, world);
    float4 viewPos = mul(worldPos, view);
    float4 projPos = mul(viewPos, proj);

    o.position = projPos;
    o.normal = input.normal;
    o.uv = input.uv;

    return o;
}

