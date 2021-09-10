#include "SimpleHeader.hlsli"
#include "MaterialHeader.hlsli"

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, pos);
    output.UV = uv;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 color = saturate(ambient + diffuse.rgb + specular);
    float4 texColor = tex.Sample(samplerState, input.UV);

    return float4(color * texColor.rgb, diffuse.a * texColor.a);
}