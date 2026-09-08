#version 450

layout(location=0) in vec2 InPosition;


layout(location = 0) out vec2  UV;
layout(location = 1) out vec3  CameraPos;
layout(location = 2) out vec3  Fog;




layout (set = 0, binding = 0) uniform Transformations
{
    mat4  View;
    mat4  Projection;
    vec3  CameraPosition;
    vec3  FogCoefficent;
}transform;


void main()
{

    mat4 Model   = mat4(1.0f);
    Model[0][0]  = 60.0f;
    Model[1][1]  = 1.0f;
    Model[2][2]  = 60.0f;

    Model[3][0] = transform.CameraPosition.x;
    Model[3][2] = transform.CameraPosition.z;


    mat4 MVP = transform.Projection * transform.View * Model;
    gl_Position = MVP *  vec4(InPosition.x, 0.0f, InPosition.y, 1.0f);
    

    UV           = vec3(Model * vec4(InPosition.x, 0.0f, InPosition.y, 1.0f)).xz;
    CameraPos    = transform.CameraPosition;
    Fog          = transform.FogCoefficent;

}
