#version 330 core

layout (location = 0) out vec4 a_Colour;

in VertexData
{
	vec4 Position;
	vec4 Colour;
} fin;

void main()
{
	a_Colour = fin.Colour;
}