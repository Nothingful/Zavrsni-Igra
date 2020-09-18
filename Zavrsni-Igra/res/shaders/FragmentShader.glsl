#version 330 core

layout(location = 0) out vec4 color;

in vec2 TexCoords;

uniform vec3 u_SpriteColor;
uniform sampler2D u_Image;

void main()
{
	color = vec4(u_SpriteColor, 1.0) * texture(u_Image, TexCoords);
};