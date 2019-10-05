#version 330 core

layout (location = 0) out vec4 a_Colour;

uniform sampler2D tex;

in VertexData
{
	vec4 Position;
	vec4 Colour;
	vec2 UV;
	float Tex;
} fin;

void main()
{
	vec4 ts = texture(tex, fin.UV);
	if (ts.w < 0.5f)
	{
		discard;
	}
	a_Colour = fin.Colour * vec4(ts.xyz * ts.w, 1.f);
}