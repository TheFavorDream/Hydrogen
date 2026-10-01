#version 450


struct FragInfo
{
  mat3 TBN;
  vec3 FragPos;
  vec2 TexCoord0;
  vec2 TexCoord1;
};

layout(location = 0) flat in uint MaterialIndex;
layout(location = 1)      in FragInfo fragInfo;
 

layout(location = 0) out vec4  OutColor;



struct Material
{
  vec4  BaseColorFactor;
  float MetallicnessFactor;
  float RoughnessFactor;
  int   BaseColorTextureID;
  int   NormalMapTextureID;  
  int   MetallicTextureID;  
};

layout(set = 0, binding = 1) readonly buffer Materials
{
  Material materials[];
};


void main()
{

  /*
  vec3 LightDir  = normalize(-(fragInfo.FragPos - light.Position));
  vec3 NormalVec = normalize(fragInfo.TBN * (2.0f*(texture(NormalMap, fragInfo.TexCoord0).rgb) - vec3(1.0f))); 


  vec3 Ambient    = light.Ambient*light.Color;
  vec3 Diffuse    = max(dot(LightDir, NormalVec), 0.0f)*light.Color;
  

  vec3 FinalColor = (Diffuse + Ambient) * texture(BaseColor, fragInfo.TexCoord0).xyz;

  */
  
  //OutColor      = vec4(FinalColor, 1.0f);
  OutColor        = vec4(fragInfo.TexCoord0.xyx, 1.0f);//texture(BaseColor, fragInfo.TexCoord0);//vec4(abs(normalize(fragInfo.FragPos)), 1.0f);
  gl_FragDepth  = 1.0f - gl_FragCoord.z;
}