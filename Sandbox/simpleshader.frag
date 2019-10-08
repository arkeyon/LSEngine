#version 330 core

layout (location = 0) out vec4 a_Colour;

uniform vec4 u_Colour;

in VertexData
{
	vec3 Position;
	vec4 Colour;
	vec2 UV;
	float Tex;
} fin;

void main()
{
	a_Colour = u_Colour * 2.f / length(fin.Position - vec3(0.f, 3.f, 0.f));
}