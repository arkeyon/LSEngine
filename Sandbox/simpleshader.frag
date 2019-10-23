#version 330 core

layout (location = 0) out vec4 a_Colour;

uniform vec3 lightpos = vec3(2.f, 7.f, 10.f);
uniform vec3 u_EyeDir;

uniform vec4 u_Color = vec4(1.f, 1.f, 1.f, 1.f);
uniform vec3 u_AmbientColor = 0.5f * vec3(1.f, 1.f, 1.f);
uniform vec3 u_DiffuseColor = 0.5f * vec3(1.f, 1.f, 1.f);
uniform vec3 u_SpecularColor = 0.5f * vec3(1.f, 1.f, 1.f);
uniform float u_Shininess = 10.f;

in VertexData
{
	vec3 Position;
	vec4 Colour;
	vec3 Normal;
	vec2 UV;
	float Tex;
} fin;

void main()
{
	vec3 lightdir = (fin.Position - lightpos);
	vec3 R = reflect(-normalize(lightdir), -fin.Normal);
	float spec = clamp(dot(R, u_EyeDir), 0.f, 1.f);
	
	float dist = length(lightdir);
	a_Colour = u_Color * vec4(fin.Colour.xyz * (u_AmbientColor + clamp(dot(-fin.Normal, normalize(lightdir)), 0.f, 1.f) * u_DiffuseColor + pow(spec, u_Shininess) * u_SpecularColor) / dist / dist * 40.f, 1.f);
}