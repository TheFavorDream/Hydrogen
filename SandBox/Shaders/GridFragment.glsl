#version 450

layout(location=0) out vec4 Color;
layout(location=1) out vec4 Normal;


layout(location=0) in vec2  UV;
layout(location=1) in vec3  CameraPos;
layout(location=2) in vec3  Fog;




void main()
{

	float LOD = floor(abs(CameraPos.y)/3.0f);

	LOD = clamp(LOD, 0.5f, 30.0f);

	vec3 GridColor = vec3(0.5f);
	vec3 XAxisColor = vec3(1.0f, 0.0f, 0.0f);
	vec3 ZAxisColor = vec3(0.0f, 0.0f, 1.0f);


	vec2 Coord = UV * (1.0f/LOD);
	vec2 Der   = fwidth(Coord);


	vec2 lineWidth = Der * 1.0f;
	vec2 Grid  = abs(fract(Coord - 0.5f) - 0.5f);
	vec2 lines = smoothstep(lineWidth, vec2(0.0f), Grid); 

	float Gridline = max(lines.x, lines.y);

	//World axis


	float XAxis = 1.0f - smoothstep(0.0f, max(lineWidth.x, lineWidth.y)*2.0f, abs(UV.y));
	float ZAxis = 1.0f - smoothstep(0.0f, max(lineWidth.x, lineWidth.y)*2.0f, abs(UV.x));



	//texel distance from the camera
	float D = length(vec3(UV.x, 0.0f, UV.y) - CameraPos);

	//calculate the fog:
	float FogAtten = 1.0f / (Fog.x*(D*D) + Fog.y*D + Fog.z);




	GridColor = mix(GridColor, XAxisColor, XAxis);
	GridColor = mix(GridColor, ZAxisColor, ZAxis);
	
	Color  = vec4(GridColor, Gridline) * FogAtten;



	Normal = vec4(Gridline,Gridline, 0.0f, 0.0f);
}
