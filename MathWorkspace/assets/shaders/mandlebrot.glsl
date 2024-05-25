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
	vec3 Position;
	vec4 Colour;
	vec3 Normal;
	vec2 UV;
	float Tex;
} vout;

void main()
{

	vec4 v = u_ModelMatrix * vec4(a_Position, 1.f);
	vout.Position = v.xyz;
	vout.Colour = a_Colour;
	vout.Normal = a_Normal;
	vout.UV = a_UV;
	vout.Tex = a_Tex;
	gl_Position = u_VP * v;
}

#type fragment
#version 330 core

#define product(a, b) vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x)
#define conjugate(a) vec2(a.x,-a.y)
#define divide(a, b) vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)))

layout (location = 0) out vec4 a_Colour;

uniform vec2 centre = vec2(0.f, 0.f);
uniform int julia;

uniform sampler2D tex;

in VertexData
{
	vec3 Position;
	vec4 Colour;
	vec3 Normal;
	vec2 UV;
	float Tex;
} fin;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

const int maxiterations = 1000;

void main()
{
	vec2 c = fin.Position.xy;
	vec2 z = vec2(0.f, 0.f);

	if (julia == 1)
	{
		c = centre;
		z = fin.Position.xy;
	}

	int count = 0;

	while (count < maxiterations)
	{
		z = product(z, z) + c;
		if (length(z) >= 2.f) break;
		count++;
	}
	
	if (count == maxiterations)
		a_Colour = vec4(0.f, 0.f, 0.f, 1.f);
	else
	{
		float a = count / 50.f;
		a_Colour = vec4(a, 0.f, a, 1.f);
	}
}
