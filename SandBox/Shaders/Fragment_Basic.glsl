#version 450

layout(location = 0) in  vec2 TexCoord0;
layout(location = 1) in  vec2 TexCoord1;
layout(location = 2) in  vec3 FragPos;
layout(location = 3) in  mat3 TBN;

 
layout(location = 0) out vec4  OutColor;
//layout(location = 1) out vec4  OutNormal;


layout (set = 1, binding = 0) uniform sampler2D BaseColor;
layout (set = 1, binding = 1) uniform sampler2D NormalMap;
layout (set = 1, binding = 2) uniform sampler2D Roughness; 


layout(set = 2, binding = 0) uniform Light
{
  vec3 Position;
  vec3 Color;
  vec3 Ambient;
  vec3 Diffuse;
  vec3 Specular;

} light;



void main()
{

  /*
  vec3 LightDir  = normalize(-(FragPos - light.Position));
  vec3 NormalVec = normalize(TBN * (2.0f*(texture(NormalMap, TexCoord0).rgb) - vec3(1.0f))); 


  vec3 Ambient    = light.Ambient*light.Color;
  vec3 Diffuse    = max(dot(LightDir, NormalVec), 0.0f)*light.Color;
  

  vec3 FinalColor = (Diffuse + Ambient) * texture(BaseColor, TexCoord0).xyz;
  */
  
  
  //OutColor      = vec4(FinalColor, 1.0f);
  OutColor      = vec4(TexCoord0.xyx, 1.0f);
  gl_FragDepth  = 1.0f - gl_FragCoord.z;
}