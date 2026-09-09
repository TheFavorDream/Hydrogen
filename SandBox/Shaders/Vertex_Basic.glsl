#version 450

layout(location = 0) in vec3  Position;
layout(location = 1) in vec3  Normals;
layout(location = 2) in vec4  Tangents;
layout(location = 3) in vec2  TexCoord0;
layout(location = 4) in vec2  TexCoord1;




layout(location = 0) out  vec2 Out_TexCoord0;
layout(location = 1) out  vec2 Out_TexCoord1;
layout(location = 2) out  vec3 Out_FragPos;
layout(location = 3) out  mat3 Out_TBN;




layout (set = 0, binding = 0) uniform Transformations
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
}transform;



void main() {




    vec3 NormalVec    = mat3(transform.Model) * Normals;
    vec3 TangentVec   = mat3(transform.Model) * Tangents.xyz;
    vec3 BiTangentVec = mat3(transform.Model) * cross(Normals, vec3(Tangents));

    
    Out_TexCoord0 = TexCoord0;
    Out_TexCoord1 = TexCoord1; 
    Out_FragPos   = vec3(transform.Model * vec4(Position, 1.0f));
    Out_TBN       = mat3(TangentVec, BiTangentVec, NormalVec);


    gl_Position = transform.Projection * transform.View * transform.Model * vec4(Position, 1.0);

}