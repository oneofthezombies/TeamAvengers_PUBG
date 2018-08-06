//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// ShadowMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// CreateShadow
//--------------------------------------------------------------//
string ShadowMapping_CreateShadow_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Torus.3ds";

texture ShadowMap_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {2048,2048};
   string Format="D3DFMT_R32F";
   float  ClearDepth=1.000000;
   int    ClearColor=-1;
>;
float4x4 World : World;
float4x4 LightView
<
   string UIName = "gLightViewMatrix";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
> = float4x4( 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00 );
float4x4 LightProjection : Projection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float4 ClipPosition : TEXCOORD1;
};

VS_OUTPUT ShadowMapping_CreateShadow_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
   Output.Position = mul( Input.Position, World);
   Output.Position = mul( Output.Position, LightView);
   Output.Position = mul( Output.Position, LightProjection);
   
   Output.ClipPosition = Output.Position;
   
   return( Output );
   
}




struct PS_INPUT 
{
   float4 ClipPosition : TEXCOORD1;
};

float4 ShadowMapping_CreateShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{   
   float depth = Input.ClipPosition.z / Input.ClipPosition.w;
   
   return( float4( depth.xxx, 1 ) );
}




//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
   pass CreateShadow
   <
      string Script = "RenderColorTarget0 = ShadowMap_Tex;"
                      "ClearColor = (255, 255, 255, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_2_0 ShadowMapping_CreateShadow_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ShadowMapping_CreateShadow_Pixel_Shader_ps_main();
   }

}

