R"rawliteral(

#version 330 core

in  vec3 Normal;
in  vec2 TexCoords;
out vec4 FragColor;


struct Material{

	sampler2D BaseColor;
	sampler2D NormalMap;
	sampler2D Metallic;

	float Metallicness;
	float Roughness;
	vec3  BaseColorFactor;
};

uniform Material material;

void main ()
{
	//FragColor   = vec4(Normal, 1.0f);
	FragColor =   vec4(material.BaseColorFactor, 1.0f) * texture(material.BaseColor, TexCoords);
}


)rawliteral"