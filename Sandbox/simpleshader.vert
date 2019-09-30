#version 330 core

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Colour;

out VertexData
{
	vec4 Position;
	vec4 Colour;
} vout;

void main()
{
	vout.Position = a_Position;
	vout.Colour = a_Colour;

	gl_Position = a_Position;
}