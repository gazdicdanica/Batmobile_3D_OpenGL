#version 330 core 

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex;
out vec2 chTex;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);;
	vec2 scaledCoords = (inPos.xy + vec2(1.0)) / 2.0;
	float scale = 1.9;

	chTex = (scaledCoords - vec2(0.5)) * scale + vec2(0.5);

}