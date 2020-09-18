#version 330 core

in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 color;

uniform sampler2D u_Sprite;

void main()
{
    color = (texture(u_Sprite, TexCoords) * ParticleColor);
}  