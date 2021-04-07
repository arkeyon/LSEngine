#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Colour;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec2 a_UV;
layout (location = 4) in float a_Tex;

uniform mat4 u_VP = mat4(1.f);
uniform mat4 u_ModelMatrix = mat4(1.f);

out VertexData
{
	vec4 Position;
	vec4 Colour;
	vec3 Normal;
	vec2 UV;
	float Tex;
} vout;

void main()
{

	vec4 v = u_VP * u_ModelMatrix * vec4(a_Position, 1.f);
	vout.Position = v;
	vout.Colour = a_Colour;
	vout.Normal = a_Normal;
	vout.UV = a_UV;
	vout.Tex = a_Tex;
	gl_Position = v;
}

#type fragment
#version 330 core

layout (location = 0) out vec4 a_Colour;

uniform vec3 lightpos = vec3(100.f, 100.f, 180.f);
uniform vec3 u_EyeDir;

uniform vec4 u_Color = vec4(1.f, 1.f, 1.f, 1.f);
uniform vec3 u_AmbientColor = vec3(1.f, 1.f, 1.f);
uniform vec3 u_DiffuseColor = vec3(1.f, 1.f, 1.f);
uniform vec3 u_SpecularColor = vec3(1.f, 1.f, 1.f);
uniform float u_Shininess = 10.f;

uniform sampler2D tex;

in VertexData
{
	vec4 Position;
	vec4 Colour;
	vec3 Normal;
	vec2 UV;
	float Tex;
} fin;

void main()
{
	a_Colour = vec4(1.f / gl_FragCoord.z * vec3(1.f, 1.f, 1.f), 1.f) / 5.f;
}
