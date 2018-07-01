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
}; 

VS_OUTPUT VS( VS_INPUT IN ) 
{ 
   VS_OUTPUT OUT;	 

   float4 oPos = mul( mul( IN.Position, World ), View ); 
   OUT.Position = mul( oPos, Projection ); 
   OUT.Normal = IN.Normal;
   OUT.TexCoord.xy = IN.TexCoord.xy; 

   return OUT;
}; 

float4  PS( VS_OUTPUT vout ) : COLOR 
{ 
   float4 color = tex2D(Heightmap_Diffuse_Sampler, vout.TexCoord );
   //if(bLight) 
   //{ 
   //  float bump = max(0, dot(normalize(vout.Normal), reflection(lightDirection)));
   //  color.rgb = color.rgb * bump;
   //} 
   if(bEmissiveColor)
     color.rgb += EmissiveColor.rgb;
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
