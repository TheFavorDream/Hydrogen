#type:Vertex
#version 330 core

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normals;
layout(location=2) in vec2 TEX_COORDS;
layout(location=3) in vec4 Tangents;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection; 


out vec3 Normal;
out vec2 TextureCoord;

void main()
{
	Normal = Normals;
	TextureCoord = TEX_COORDS;
	gl_Position = Projection * View * Model *  vec4(Position, 1.0f);
}

#type:Fragment
#version 330 core

in vec2 TextureCoord;
in vec3 Normal;
out vec4 FragColor;

uniform sampler2D TEX;

void main ()
{

		FragColor = texture(TEX, TextureCoord);
		//FragColor = vec4(Normal, 1.0f);
}
