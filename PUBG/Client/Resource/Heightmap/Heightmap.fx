// Heightmap.fx

float4x4 World;
float4x4 View; 
float4x4 Projection; 

bool bEmissiveColor = false; 
bool bLight = true; 
float3 lightDirection = { 0, 0, -1 }; 

float4 DiffuseColor  = { 1.000000f, 1.000000f, 1.000000f, 1.000000f }; 
float  SpecularPower = 10.000000f; 
float4 SpecularColor = { 0.000000f, 0.000000f, 0.000000f, 1.000000f }; 
float4 EmissiveColor = { 0.000000f, 0.000000f, 0.000000f, 1.000000f }; 

float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;

texture Heightmap_Diffuse_Tex; 
texture ShadowMap_Tex;

sampler2D Heightmap_Diffuse_Sampler = sampler_state
{ 
   Texture = <Heightmap_Diffuse_Tex>;
   MinFilter = Linear; 
   MagFilter = Linear; 
   MipFilter = Linear; 
   AddressU  = Wrap;     
   AddressV  = Wrap;     
}; 

sampler2D ShadowSampler = sampler_state
{
   Texture = <ShadowMap_Tex>;
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

   OUT.ClipPosition = mul(worldPos, gLightViewMatrix);
   OUT.ClipPosition = mul(OUT.ClipPosition, gLightProjMatrix);

   return OUT;
}; 

float4  PS( VS_OUTPUT vout ) : COLOR 
{ 
   float4 color = tex2D(Heightmap_Diffuse_Sampler, vout.TexCoord );

   if(bEmissiveColor)
     color.rgb += EmissiveColor.rgb;

   float currentDepth = vout.ClipPosition.z / vout.ClipPosition.w;
   float2 uv = vout.ClipPosition.xy / vout.ClipPosition.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;

   float shadowDepth = tex2D(ShadowSampler, uv).r;
   
   if ( currentDepth > shadowDepth + 0.0000125f )
   {
      color.rgb *= 0.5f;
   }

   return  color; 
}; 

technique DefaultTechnique 
{ 
   pass p0 
   { 
      VertexShader = compile vs_3_0  VS(); 
      PixelShader  = compile ps_3_0  PS(); 
   } 
} 
