//TODO: Modify the folowing code: 

int GlobalParameter : SasGlobal 
< 
   int3 SasVersion = {1, 1, 0}; 
   string SasEffectDescription = "This is a sample effect"; 
   string SasEffectCompany = "cgdev.net"; 
>; 

float4x4 World      : WORLD <string SasBindAddress = "Sas.Skeleton.MeshToJointToWorld[0]";>; 
float4x4 View       : VIEW <string SasBindAddress = "Sas.Camera.WorldToView";>; 
float4x4 Projection : PROJECTION <string SasBindAddress = "Sas.Camera.Projection";>; 

bool bEmissiveColor = true; 
bool bLight = true; 
float3 lightDirection = { 0, 0, -1 }; 

float4 DiffuseColor  = { 1.000000f, 1.000000f, 1.000000f, 1.000000f }; 
float  SpecularPower = 10.000000f; 
float4 SpecularColor = { 0.000000f, 0.000000f, 0.000000f, 1.000000f }; 
float4 EmissiveColor = { 0.100000f, 0.100000f, 0.100000f, 1.000000f }; 

float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;

texture F_BaseBody_04_2; 
texture Map__1; 
texture Map__2; 
texture ShadowMap_Tex;

sampler2D F_BaseBody_04_2Sampler = sampler_state  // TexCoord0 
{ 
   Texture = <F_BaseBody_04_2>; 
   MinFilter = Linear; 
   MagFilter = Linear; 
   MipFilter = Linear; 
   AddressU  = Wrap;     
   AddressV  = Wrap;     
}; 

sampler2D Map__1Sampler = sampler_state  // TexCoord0 
{ 
   Texture = <Map__1>; 
   MinFilter = Linear; 
   MagFilter = Linear; 
   MipFilter = Linear; 
   AddressU  = Wrap;     
   AddressV  = Wrap;     
}; 

sampler2D Map__2Sampler = sampler_state  // TexCoord0 
{ 
   Texture = <Map__2>; 
   MinFilter = Linear; 
   MagFilter = Linear; 
   MipFilter = Linear; 
   AddressU  = Wrap;     
   AddressV  = Wrap;     
}; 

sampler2D ShadowSampler = sampler_state
{
   Texture = <ShadowMap_Tex>; 
};

struct VS_INPUT 
{ 
   float4 Position  : POSITION; 
   float2 TexCoord0 : TEXCOORD0;
   float3 N         : NORMAL;
   float3 T         : TANGENT;
   float3 B         : BINORMAL;
}; 

struct VS_OUTPUT 
{ 
   float4 Position  : POSITION; 
   float2 TexCoord0 : TEXCOORD0;
   float3 LightTangentSpace : COLOR1;
   float4 ClipPosition : TEXCOORD1;
}; 

VS_OUTPUT VS( VS_INPUT IN ) 
{ 
   //TODO: Modify the folowing code: 
   VS_OUTPUT OUT;	 
   float4 WorldPos = mul(IN.Position, World);
   float4 oPos = mul( WorldPos, View ); 
   OUT.Position = mul( oPos, Projection ); 
   
   OUT.TexCoord0.xy = IN.TexCoord0.xy; 
   
   float3x3 TBN = { IN.T, IN.B, IN.N };
   OUT.LightTangentSpace = mul( mul( TBN, (float3x3)World ), lightDirection );

  OUT.ClipPosition = mul(WorldPos, gLightViewMatrix);
  OUT.ClipPosition = mul(OUT.ClipPosition, gLightProjMatrix);

   return OUT;
}; 

float4  PS( VS_OUTPUT vout ) : COLOR 
{ 
   //TODO: Modify the folowing code: 
   float4 color = tex2D( F_BaseBody_04_2Sampler, vout.TexCoord0 ); 

   if(bLight) 
   { 
     float3 bumpNormal =  2 * tex2D( Map__2Sampler, vout.TexCoord0 ) - 1 ;
     float3 LightVector1 = normalize(vout.LightTangentSpace);
     float bump = max(0, dot( bumpNormal, LightVector1 ));
     color.rgb = color.rgb * bump;
   } 

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
