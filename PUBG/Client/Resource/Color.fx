float4x4 World; 
float4x4 View; 
float4x4 Projection; 

float4   Color;

struct VS_INPUT 
{ 
  float4 Position  : POSITION; 
}; 

struct VS_OUTPUT 
{ 
  float4 Position  : POSITION; 
}; 

VS_OUTPUT VS(VS_INPUT input) 
{ 
  VS_OUTPUT output;

  output.Position = mul(input.Position, World);
  output.Position = mul(output.Position, View);
  output.Position = mul(output.Position, Projection);

  return output;
}; 

float4  PS() : COLOR 
{ 
  return Color;
}; 

technique DefaultTechnique 
{ 
   pass p0 
   { 
      VertexShader = compile vs_3_0  VS(); 
      PixelShader  = compile ps_3_0  PS(); 
   } 
} 
