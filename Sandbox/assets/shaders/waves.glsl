#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Colour;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec2 a_UV;
layout (location = 4) in float a_Tex;

uniform mat4 u_VP = mat4(1.f);
uniform mat4 u_ModelMatrix = mat4(1.f);
uniform float u_Time = 0.f;
uniform float u_Phase = 0.f;
uniform float u_Frequency = 15.f;
uniform float u_Speed = 30.f;
uniform float u_Amplitude = 10.f;
uniform int u_Dipoles = 15;
uniform float u_Spacing = 4.f;

float PI = 3.141592f;

float angle = 1.f;

out VertexData
{
	vec3 Position;
	vec4 Colour;
	vec3 Normal;
	vec2 UV;
	float Tex;
} vout;

void main()
{

	vec4 vt = u_ModelMatrix * vec4(a_Position, 1.f);
	float v = vt.x;
	float u = vt.y;
	vec3 pos = vec3(v, u, 0.f );
	for (float i = 0; i < u_Dipoles; i++)
	{
		float dist = length(pos.xy - vec2((float(i) - u_Dipoles / 2.f) * u_Spacing, 0.f));

		if (dist < u_Speed * u_Time)
		{
			float a = u_Amplitude / float(u_Dipoles) * sin(i / PI / 2.f * u_Phase + u_Frequency * (u_Time - dist / u_Speed)) / (dist / 50.f + 1.f);
			pos += vec3(0.f, 0.f, a);
		}
	}
	vout.Position = pos;
	vout.Colour = mix(vec4(0.f, 0.2f, 0.8f, 1.f), vec4(0.f, 0.4f, 0.6f, 1.f), pos.z / 10.f);
	vout.Normal = a_Normal;
	vout.UV = a_UV;
	vout.Tex = a_Tex;
	gl_Position = u_VP * vec4(pos, 1.f);
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
uniform float u_Shininess = 100.f;

uniform sampler2D tex;

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

	float spec = pow(clamp(dot(R, u_EyeDir), 0.f, 1.f), u_Shininess);
	float diffuse = clamp(dot(-fin.Normal, normalize(lightdir)), 0.f, 1.f);

	float dist = length(lightdir);
	a_Colour = u_Color * texture2D(tex, fin.UV) * vec4(fin.Colour.xyz * (u_AmbientColor + diffuse * u_DiffuseColor + spec * u_SpecularColor) * 100.f / dist, fin.Colour.z);
}
