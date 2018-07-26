// lighting and shadow applying

float4x4 World; 
float4x4 View; 
float4x4 Projection; 

bool bEmissiveColor = false; 
bool bLight         = true; 
bool bShadow        = true;

float4   LightPos; 
float4x4 LightView;
float4x4 LightProjection;

float4 CameraPos;

float4 DiffuseColor  = { 1.000000f, 1.000000f, 1.000000f, 1.000000f }; 
float  SpecularPower = 10.000000f; 
float4 SpecularColor = { 0.000000f, 0.000000f, 0.000000f, 1.000000f }; 
float4 EmissiveColor = { 0.010000f, 0.010000f, 0.010000f, 1.000000f }; 

texture ShadowMap_Tex;

sampler2D ShadowSampler = sampler_state
{
    Texture = <ShadowMap_Tex>; 
    MinFilter = Linear; 
    MagFilter = Linear; 
    MipFilter = Linear; 
	AddressU = Border;
	AddressV = Border;
	BorderColor = float4(1.0f, 1.0f, 1.0f, 1.0f); 
};

texture Heightmap_Diffuse_Tex; 

sampler2D Heightmap_Diffuse_Sampler = sampler_state
{ 
   Texture = <Heightmap_Diffuse_Tex>;
   MinFilter = Linear; 
   MagFilter = Linear; 
   MipFilter = Linear; 
   AddressU  = Wrap;     
   AddressV  = Wrap;     
}; 

texture Heightmap_Normal_Tex;

sampler2D Heightmap_Normal_Sampler = sampler_state
{
    Texture = <Heightmap_Normal_Tex>;
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
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position          : POSITION;
    float2 TexCoord          : TEXCOORD0;
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

    vout.TexCoord = vin.TexCoord;

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
    float3 tangentNormal = tex2D(Heightmap_Normal_Sampler, vout.TexCoord).xyz;
    tangentNormal = normalize(tangentNormal * 2 - 1);

    float3 Tangent = float3(0.0f, 0.0f, 1.0f);
    float3 Binormal = float3(1.0f, 0.0f, 0.0f);

    float3x3 TBN = float3x3(normalize(Tangent),
        normalize(Binormal),
        normalize(vout.Normal));
    TBN = transpose(TBN);

    float3 worldNormal = mul(TBN, tangentNormal);

    float4 albedo = tex2D(Heightmap_Diffuse_Sampler, vout.TexCoord);
    float3 lightDir = normalize(vout.LightDirection);
    float3 diffuse = saturate(dot(worldNormal, -lightDir));
    diffuse = max(float3(0.3f, 0.3f, 0.3f), diffuse);
    diffuse = albedo.rgb * diffuse;

    float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
    float3 rgb = ambient + diffuse;

    if (bShadow)
    {
        float currentDepth = vout.ClipPosition.z / vout.ClipPosition.w;
        float2 uv = vout.ClipPosition.xy / vout.ClipPosition.w;
        uv.y = -uv.y;
        uv = uv * 0.5 + 0.5;

        float shadowDepth = tex2D(ShadowSampler, uv).r;

        if (currentDepth > shadowDepth + 0.000125f)
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
