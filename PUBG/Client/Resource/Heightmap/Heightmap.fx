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
   AddressU  = Wrap;     
   AddressV  = Wrap;    
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

struct VS_INPUT 
{ 
   float4 Position  : POSITION; 
   float3 Normal    : NORMAL;
   float2 TexCoord  : TEXCOORD;
}; 

struct VS_OUTPUT 
{ 
   float4 Position  : POSITION; 
   float3 Normal    : NORMAL;
   float2 TexCoord  : TEXCOORD;
   float4 ClipPosition : TEXCOORD1;
}; 

VS_OUTPUT VS( VS_INPUT IN ) 
{ 
   VS_OUTPUT OUT;	 

  float4 worldPos = mul( IN.Position, World );
   float4 oPos = mul(worldPos , View ); 
   OUT.Position = mul( oPos, Projection ); 
   OUT.Normal = IN.Normal;
   OUT.TexCoord.xy = IN.TexCoord.xy; 

   OUT.ClipPosition = mul(worldPos, LightView);
   OUT.ClipPosition = mul(OUT.ClipPosition, LightProjection);

   return OUT;
}; 

float4  PS( VS_OUTPUT vout ) : COLOR 
{ 
    float4 albedo = tex2D(Heightmap_Diffuse_Sampler, vout.TexCoord); 

    float3 color = albedo.rgb;
    float3 specular = 0;

    if (bEmissiveColor)
    {
        color += EmissiveColor.rgb;
    }

    color = color + specular;

    if (bShadow)
    {
        float currentDepth = vout.ClipPosition.z / vout.ClipPosition.w;
        float2 uv = vout.ClipPosition.xy / vout.ClipPosition.w;
        uv.y = -uv.y;
        uv = uv * 0.5 + 0.5;

        float shadowDepth = tex2D(ShadowSampler, uv).r;
   
        if (currentDepth > shadowDepth + 0.0000125f)
        {
            color *= 0.5f;
        }
    }

    return  float4(color, 1.0f); 
}; 

technique DefaultTechnique 
{ 
   pass p0 
    { 
      VertexShader = compile vs_3_0  VS(); 
      PixelShader  = compile ps_3_0  PS(); 
   } 
} 
