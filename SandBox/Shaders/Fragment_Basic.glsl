#version 450

layout(location = 0) in  vec2 TexCoord0;
layout(location = 1) in  vec2 TexCoord1;
layout(location = 2) in  vec3 FragPos;
layout(location = 3) in  mat3 TBN;
 
layout(location = 0) out vec4  OutColor;
layout(location = 1) out vec4  OutNormal;


layout (set = 1, binding = 0) uniform sampler2D BaseColor;
layout (set = 1, binding = 1) uniform sampler2D NormalMap;
layout (set = 1, binding = 2) uniform sampler2D Roughness; 


//Light Dir:

vec3  LightPosition = vec3(4.0f, 0.0f, 0.0f);
float AmbientLight  = 0.02f;

void main()
{

  vec3 LightDir = normalize(-(FragPos - LightPosition));


  vec3 NormalVec    =  2.0f*(texture(NormalMap, TexCoord0).rgb) - vec3(1.0f); 

  NormalVec = normalize(TBN * NormalVec);

  float Diffuse   = max(dot(LightDir, NormalVec), 0.0f);
 


  vec3 FinalColor = texture(BaseColor, TexCoord0).xyz*Diffuse + (AmbientLight*texture(BaseColor, TexCoord0).xyz);


  OutColor      = vec4(FinalColor, 1.0f);
  OutNormal     = vec4(NormalVec, 1.0f);
  gl_FragDepth  = 1.0f - gl_FragCoord.z;
}