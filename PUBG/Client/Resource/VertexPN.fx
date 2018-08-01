// lighting and shadow applying

float4x4 World;
float4x4 View;
float4x4 Projection;

bool bEmissiveColor = false;
bool bLight = true;
bool bShadow = true;

float4   LightPos;
float4x4 LightView;
float4x4 LightProjection;

float4 CameraPos;

float4 DiffuseColor = { 1.000000f, 1.000000f, 1.000000f, 1.000000f };
float  SpecularPower = 20.000000f;
float4 SpecularColor = { 0.000000f, 0.000000f, 0.000000f, 1.000000f };
float4 EmissiveColor = { 0.010000f, 0.010000f, 0.010000f, 1.000000f };

texture ShadowMap_Tex;

sampler2D ShadowSampler = sampler_state
{
    Texture = <ShadowMap_Tex>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_INPUT
{
    float4 Position : POSITION;
    float3 Normal   : NORMAL;
};

struct VS_OUTPUT
{
    float4 Position          : POSITION;
    float3 LightDirection    : TEXCOORD1;
    float3 ViewDirection     : TEXCOORD2;
    float3 Normal            : TEXCOORD5;
    float4 ClipPosition      : TEXCOORD6;
    float  Diffuse           : TEXCOORD7;
};

VS_OUTPUT VS(VS_INPUT vin)
{
    VS_OUTPUT vout;

    float4 worldPos = mul(vin.Position, World);

    vout.Position = mul(mul(worldPos, View), Projection);

    vout.LightDirection = normalize(worldPos.xyz - LightPos.xyz);

    vout.ViewDirection = normalize(worldPos.xyz - CameraPos.xyz);

    vout.Normal = normalize(mul(vin.Normal, (float3x3)World));

    vout.ClipPosition = mul(mul(worldPos, LightView), LightProjection);

    float3 worldNormal = normalize(mul(vin.Normal, (float3x3)World));
    vout.Diffuse = dot(-vout.LightDirection, worldNormal);

    return vout;
};

float4  PS(VS_OUTPUT vout) : COLOR
{
    float3 ambient = float3(0.1f, 0.1f, 0.1f);
    float3 rgb = ambient + vout.Diffuse;

    if (bShadow)
    {
        float currentDepth = vout.ClipPosition.z / vout.ClipPosition.w;
        float2 uv = vout.ClipPosition.xy / vout.ClipPosition.w;
        uv.y = -uv.y;
        uv = uv * 0.5 + 0.5;

        float shadowDepth = tex2D(ShadowSampler, uv).r;

        if (currentDepth > shadowDepth + 0.0000125f)
        {
            rgb *= 0.7f;
        }
    }

    return float4(rgb, 1);
};



technique DefaultTechnique 
{ 
   pass p0 
   { 
      VertexShader = compile vs_3_0  VS(); 
      PixelShader  = compile ps_3_0  PS(); 
   } 
} 
