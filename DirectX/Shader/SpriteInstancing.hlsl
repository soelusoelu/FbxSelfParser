Texture2D texDecal : register(t0);
SamplerState samLinear : register(s0);

struct VS_IN
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD0;
    matrix wp : MATRIX; //インスタンスごとに設定されるワールド*プロジェクション行列
    float4 color : COLOR; //インスタンスごとに設定される色
    float4 uv : TEXCOORD1; //インスタンスごとに設定されるUV
    uint InstanceID : SV_InstanceID; //インスタンスID
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(input.wp, input.Pos);
    output.color = input.color;
    float4 uv = input.uv;
    output.UV = input.UV * float2(uv.z - uv.x, uv.w - uv.y) + float2(uv.x, uv.y);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 col = texDecal.Sample(samLinear, input.UV);
    col *= input.color;
    return col;
}