#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Colour;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in float a_Tex;

uniform mat4 u_VP = mat4(1.f);
uniform mat4 u_ModelMatrix = mat4(1.f);

out VertexData
{
	vec4 Position;
	vec4 Colour;
	vec2 UV;
	float Tex;
} vout;

void main()
{
	vec4 v = u_ModelMatrix * vec4(a_Position, 1.f);
	vout.Position = v;
	vout.Colour = a_Colour;
	vout.UV = a_UV;
	vout.Tex = a_Tex;
	gl_Position = u_VP * v;
}