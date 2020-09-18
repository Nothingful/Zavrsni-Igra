#version 330 core

//layout (location = 0) in vec2 a_Vertex; // vertex coordinates
//layout (location = 1) in vec2 a_TexCoords; // texture coordinates
layout (location = 0) in vec4 a_Vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
    TexCoords = a_Vertex.zw;
    gl_Position = u_Projection * u_Model * vec4(a_Vertex.xy, 0.0, 1.0);
	//TexCoords = a_TexCoords;
    //gl_Position = u_Projection * u_Model * vec4(a_Vertex, 0.0, 1.0);
};