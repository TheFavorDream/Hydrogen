#version 440

layout(location=0) in vec2 InPosition;

void main()
{
	gl_Position = vec4(InPosition, 0.0f, 1.0f);
}
