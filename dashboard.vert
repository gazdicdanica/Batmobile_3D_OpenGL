#version 330 core

layout(location = 0) in vec2 inPos; // 2D position
layout(location = 1) in vec4 inCol; // Color
out vec4 channelCol; // Output color to fragment shader

uniform int one;
uniform float rotationAngle;

uniform mat4 uM; // Model matrix
uniform mat4 uV; // View matrix
uniform mat4 uP; // Projection matrix

void main()
{
    if(one == 1){
		if(gl_VertexID == 0){
			gl_Position =  uP * uV * uM * vec4(inPos, 0.0, 1.0);
		}
		else{
			vec2 center = vec2(-0.55, -0.45);
			vec2 translatedPosition = inPos - center;
			float cosA = cos(radians(rotationAngle));
			float sinA = sin(radians(rotationAngle));
			mat2 rotationMatrix = mat2(cosA, -sinA, sinA, cosA);
			vec2 final = rotationMatrix * translatedPosition + center;
			if(final.x < -0.6) {final.x = -0.6;}
			if(final.x > -0.5) { final.x = -0.5;}
			if(final.y > -0.56) {final.y = -0.56;}
			gl_Position = vec4(final, 0.0, 1.0);
			
		}
		
	}else{
		vec3 pos3D = vec3(inPos.x,inPos.y, 0.0);
		gl_Position = uP * uV * uM * vec4(pos3D, 1.0);
    }
    channelCol = inCol; // Pass color to fragment shader
}
