#version 330 core 

in vec4 channelCol; 
out vec4 outCol; 
uniform vec3 uCol;

void main() 
{
	outCol = vec4(channelCol.r + uCol.r, channelCol.g + uCol.g, channelCol.b + uCol.b, channelCol.a);

}