#version 450

layout(location = 0) in vec3  Position;
layout(location = 1) in vec3  Normals;
layout(location = 2) in vec4  Tangents;
layout(location = 3) in vec2  TexCoord0;
layout(location = 4) in vec2  TexCoord1;



layout(location = 0) out  vec3 fragColor;
layout(location = 1) out  vec2 Out_TexCoord0;
layout(location = 2) out  vec2 Out_TexCoord1;

layout (set = 0, binding = 0) uniform Transformations
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
}transform;



void main() {


    Out_TexCoord0 = TexCoord0;
    Out_TexCoord1 = TexCoord1; 

    gl_Position = transform.Projection * transform.View * transform.Model * vec4(Position, 1.0);
    fragColor   = Normals;
}