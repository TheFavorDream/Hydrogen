@Name:Basic_VS
@Id:12001
@Type:Vertex

#version 330 core

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normals;
layout(location=2) in vec4 Tangents;
layout(location=3) in vec2 TEX_COORDS_0;
layout(location=4) in vec2 TEX_COORDS_1;
layout(location=5) in vec2 COLOR_0;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection; 

out vec3 Normal;
out vec2 TexCoords;

//Main Function:
void main()
{
	Normal =    Normals;
	TexCoords = TEX_COORDS_0;
	gl_Position = Projection * View * Model *  vec4(Position, 1.0f);
}

@Name:Basic_FS
@Id:12002
@Type:Fragment

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
	FragColor   = texture(material.NormalMap, TexCoords);
	//FragColor =   vec4(material.BaseColorFactor, 1.0f) * texture(material.BaseColor, TexCoords);
}