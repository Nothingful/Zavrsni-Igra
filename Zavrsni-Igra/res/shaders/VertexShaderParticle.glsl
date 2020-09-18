#version 330 core
layout (location = 0) in vec4 a_Vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 u_Projection;
uniform vec2 u_Offset;
uniform vec4 u_Color;

void main()
{
    float scale = 10.0f;
    TexCoords = a_Vertex.zw;
    ParticleColor = u_Color;
    gl_Position = u_Projection * vec4((a_Vertex.xy * scale) + u_Offset, 0.0, 1.0);
}