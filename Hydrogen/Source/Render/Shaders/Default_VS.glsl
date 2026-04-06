R"rawliteral(
#version 330 core

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normals;
layout(location=2) in vec2 TEX_COORDS_0;
layout(location=3) in vec2 TEX_COORDS_1;
layout(location=4) in vec4 Tangents;
layout(location=5) in vec2 COLOR_0;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection; 

out vec3 Normal;
out vec2 TexCoords;

void main()
{
	Normal =    Normals;
	TexCoords = TEX_COORDS_0;
	gl_Position = Projection * View * Model *  vec4(Position, 1.0f);
}
)rawliteral"