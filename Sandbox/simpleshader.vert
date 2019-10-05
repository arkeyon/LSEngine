#version 330 core

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Colour;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in float a_Tex;

out VertexData
{
	vec4 Position;
	vec4 Colour;
	vec2 UV;
	float Tex;
} vout;

void main()
{
	vout.Position = a_Position;
	vout.Colour = a_Colour;
	vout.UV = a_UV;
	vout.Tex = a_Tex;
	gl_Position = a_Position;
}