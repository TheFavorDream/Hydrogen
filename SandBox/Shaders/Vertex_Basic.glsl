#version 450

layout(location = 0) in vec3  Position;
layout(location = 1) in vec3  Normals;
layout(location = 2) in vec4  Tangents;
layout(location = 3) in vec2  TexCoord0;
layout(location = 4) in vec2  TexCoord1;
//layout(location = 5) in vec3  Color01;

struct Primitive
{
    mat4 Model;
};

layout(set = 0, binding = 0) readonly buffer Primitives
{
    Primitive primitives[];
};

layout (set = 1, binding = 0) uniform Camera
{
    mat4 View;
    mat4 Projection;
}camera;


layout(push_constant) uniform IDs
{
    int PrimitiveIndex;
    int MaterialIndex;
} id;

struct FragInfo
{
    mat3 TBN;
    vec3 FragPos;
    vec2 TexCoord0;
    vec2 TexCoord1;
};

layout(location = 0) out int      MaterialIndex; 
layout(location = 1) out FragInfo OutfragInfo;


void main() {


    Primitive primitive = primitives[id.PrimitiveIndex];

    MaterialIndex = id.MaterialIndex;

    gl_Position = camera.Projection * camera.View  *  primitive.Model * vec4(Position, 1.0);


    vec3 NormalVec    = mat3(primitive.Model) * Normals;
    vec3 TangentVec   = mat3(primitive.Model) * Tangents.xyz;
    vec3 BiTangentVec = mat3(primitive.Model) * cross(Normals, vec3(Tangents));

    
    OutfragInfo.TexCoord0 = TexCoord0;
    OutfragInfo.TexCoord1 = TexCoord1; 
    OutfragInfo.FragPos   = vec3(primitive.Model * vec4(Position, 1.0f));
    OutfragInfo.TBN       = mat3(TangentVec, BiTangentVec, NormalVec);



}