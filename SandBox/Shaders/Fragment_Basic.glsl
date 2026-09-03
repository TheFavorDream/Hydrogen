#version 450

layout(location = 0) in  vec3 Color;
layout(location = 1) in  vec2 TexCoord0;
layout(location = 2) in  vec2 TexCoord1;

layout(location = 0) out vec4  OutColor;
layout(location = 1) out vec4  OutNormal;





layout (set = 1, binding = 0) uniform sampler2D BaseColor;
layout (set = 1, binding = 1) uniform sampler2D NormalMap;
layout (set = 1, binding = 2) uniform sampler2D Roughness; 


void main()
{

  OutColor      = texture(BaseColor, TexCoord0);
  OutNormal     = texture(NormalMap, TexCoord0);
  
  gl_FragDepth  = 1.0f - gl_FragCoord.z;
}