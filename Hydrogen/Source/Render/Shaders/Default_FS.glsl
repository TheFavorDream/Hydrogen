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
};

uniform Material material;

void main ()
{
	FragColor   = vec4(Normal, 1.0f);
	//FragColor = texture(material.BaseColor, TexCoords);
}


)rawliteral"