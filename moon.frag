#version 330 core

in vec2 chTex; // Texture coordinates
out vec4 outCol;

uniform sampler2D uTex; // Texture unit
uniform vec4 color;

void main()
{
    vec4 moonColor = texture(uTex, chTex); // Fetch the color from the moon texture
    vec4 finalColor = mix(color, moonColor, moonColor.a);

    outCol = finalColor;
}
