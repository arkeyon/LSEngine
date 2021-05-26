#include "lsepch.h"
#include "Meshfactory.h"

#include <glm/gtc/matrix_transform.hpp>

namespace LSE
{

	const glm::ivec2 tvertices[]
	{
		glm::ivec2(0, 0),
		glm::ivec2(0, 1),
		glm::ivec2(1, 1),
		glm::ivec2(1, 0)
	};

	const int32_t tindices[]
	{
		0, 3, 1, 1, 3, 2
	};

	Ref<Mesh> MeshFactory::cubeSphere(const float& radius, const int& detail, glm::vec4 colour, std::vector<int>* strides)
	{
		using namespace glm;
		Ref<Mesh> mesh = griddedCubeCenter(1.f, detail, colour, strides);
		for (int i = 0; i < mesh->m_VerticesCount; i++)
		{
			vertex_t& v = mesh->m_Vertices[i];
			vec3& p = v.a_Position;
			vec3 n = p / sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
			v.a_Position = n * radius;
			v.a_Normal = n;
		}

		return mesh;
	}

	Ref<Mesh> MeshFactory::cubeEllipsoid(const float& a, const float& b, const float& c, const int& detail, glm::vec4 colour, std::vector<int>* strides)
	{
		using namespace glm;

		float av = (1.f / a + 1.f / b + 1.f / c);

		int ad = (int)(av * a * detail);
		int bd = (int)(av * b * detail);
		int cd = (int)(av * c * detail);

		Ref<Mesh> mesh = griddedRectCenter(a, b, c, ad, bd, cd, colour, strides);
		for (int i = 0; i < mesh->m_VerticesCount; i++)
		{
			vertex_t& v = mesh->m_Vertices[i];
			vec3& p = v.a_Position;
			vec3 n = v.a_Position / sqrtf(p.x * p.x / a / a + p.y * p.y / b / b + p.z * p.z / c / c);
			v.a_Position = n;
			v.a_Normal = normalize(n);
		}

		return mesh;
	}

	Ref<Mesh> MeshFactory::sphere(const float& radius, const int& detail, glm::vec4 colour)
	{
		const int width = detail * 2;
		const int height = detail;
		return sphere(radius, width, height, colour);
	}

	Ref<Mesh> MeshFactory::cubeCorner(float size, glm::vec4 colour)
	{
		return MeshFactory::rectCorner(size, size, size, colour);
	}

	Ref<Mesh> MeshFactory::cubeCenter(float size, glm::vec4 colour)
	{
		return MeshFactory::rectCenter(size, size, size, colour);
	}

	float a(float f)
	{
		return 1.f - fabsf(f * 2.f - 1.f);
	}

	float b(float f)
	{
		return a(fmodf(2.f * f, 2.f) / 2.f);
	}

	Ref<Mesh> MeshFactory::sphere(const float& radius, const int& w, const int& h, glm::vec4 colour)
	{

		const int width = w + 1;
		const int height = h + 1;

		Ref<Mesh> mesh = MakeRef<Mesh>(width * height, w * h * 6);

		float cor = float(width % 2) * 0.5f;

		for (int y = 0; y < height; y++)
		{
			float pitch = (y - (height - 1.f) / 2.f) / (height - 1.f) * glm::pi<float>();

			float rr = cos(pitch);

			for (int x = 0; x < width; x++)
			{
				float yaw = x / (width - 1.f) * 2.f * glm::pi<float>();

				mesh->m_Vertices[x + y * width].a_Colour = colour;
				mesh->m_Vertices[x + y * width].a_Tex = 0.f;
				mesh->m_Vertices[x + y * width].a_Normal = glm::vec3(cos(yaw) * rr, sin(yaw) * rr, sin(pitch));
				mesh->m_Vertices[x + y * width].a_Position = mesh->m_Vertices[x + y * width].a_Normal * radius;
				mesh->m_Vertices[x + y * width].a_UV = glm::vec2(b((float)(x + cor) / (width - 1.f)), b((float)y / (height - 1.f)));

			}
		}



		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				for (int n = 0; n < 6; n++)
				{
					glm::ivec2 v = tvertices[tindices[n]];
					mesh->m_Indices[(x + y * w) * 6 + n] = (v.x + x) + (v.y + y) * width;
				}
			}
		}

		return mesh;
	}


	Ref<Mesh> MeshFactory::colourRectCorner(float width, float height, float depth)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(24, 36);

		using namespace glm;
		mesh->m_Vertices[0].a_Position = vec3(0.f, 0.f, 0.f); //BOTTOM
		mesh->m_Vertices[1].a_Position = vec3(0.f, depth, 0.f);
		mesh->m_Vertices[2].a_Position = vec3(width, depth, 0.f);
		mesh->m_Vertices[3].a_Position = vec3(width, 0.f, 0.f);
		mesh->m_Vertices[0].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[1].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[2].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[3].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[0].a_Colour = vec4(1.f, 0.f, 0.f, 1.f); //RED
		mesh->m_Vertices[1].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[2].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[3].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		//mesh->m_Vertices[0].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //RED
		//mesh->m_Vertices[1].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[2].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[3].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[0].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[1].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[2].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[3].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[4].a_Position = vec3(0.f, depth, height); //TOP
		mesh->m_Vertices[5].a_Position = vec3(0.f, 0.f, height);
		mesh->m_Vertices[6].a_Position = vec3(width, 0.f, height);
		mesh->m_Vertices[7].a_Position = vec3(width, depth, height);
		mesh->m_Vertices[4].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[5].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[6].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[7].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[4].a_Colour = vec4(0.f, 1.f, 0.f, 1.f); //GREEN
		mesh->m_Vertices[5].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[6].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[7].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		//mesh->m_Vertices[4].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //GREEN
		//mesh->m_Vertices[5].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[6].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[7].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[4].a_UV = vec2(0.01f, 0.01f);
		mesh->m_Vertices[5].a_UV = vec2(0.01f, 0.99f);
		mesh->m_Vertices[6].a_UV = vec2(0.99f, 0.99f);
		mesh->m_Vertices[7].a_UV = vec2(0.99f, 0.01f);

		mesh->m_Vertices[8 ].a_Position = vec3(0.f, 0.f, height); //LEFT
		mesh->m_Vertices[9 ].a_Position = vec3(0.f, depth, height);
		mesh->m_Vertices[10].a_Position = vec3(0.f, depth, 0.f);
		mesh->m_Vertices[11].a_Position = vec3(0.f, 0.f, 0.f);
		mesh->m_Vertices[8 ].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[9 ].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[10].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[11].a_Normal = vec3(-1.f, 0.f, 0.f);
		//mesh->m_Vertices[8 ].a_Colour = vec4(0.f, 0.f, 1.f, 1.f); //BLUE
		//mesh->m_Vertices[9 ].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		//mesh->m_Vertices[10].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		//mesh->m_Vertices[11].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[8 ].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //BLUE
		mesh->m_Vertices[9 ].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[10].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[11].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[8 ].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[9 ].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[10].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[11].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[12].a_Position = vec3(width, 0.f, 0.f); //RIGHT
		mesh->m_Vertices[13].a_Position = vec3(width, depth, 0.f);
		mesh->m_Vertices[14].a_Position = vec3(width, depth, height);
		mesh->m_Vertices[15].a_Position = vec3(width, 0.f, height);
		mesh->m_Vertices[12].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[13].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[14].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[15].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[12].a_Colour = vec4(1.f, 1.f, 0.f, 1.f); //YELLOW
		mesh->m_Vertices[13].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[14].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[15].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		//mesh->m_Vertices[12].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //YELLOW
		//mesh->m_Vertices[13].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[14].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[15].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[12].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[13].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[14].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[15].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[16].a_Position = vec3(0.f, depth, 0.f); //BACK
		mesh->m_Vertices[17].a_Position = vec3(0.f, depth, height);
		mesh->m_Vertices[18].a_Position = vec3(width, depth, height);
		mesh->m_Vertices[19].a_Position = vec3(width, depth, 0.f);
		mesh->m_Vertices[16].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[17].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[18].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[19].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[16].a_Colour = vec4(0.f, 1.f, 1.f, 1.f); //AQUA
		mesh->m_Vertices[17].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[18].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[19].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[16].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //AQUA
		//mesh->m_Vertices[17].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[18].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[19].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[16].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[17].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[18].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[19].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[20].a_Position = vec3(0.f, 0.f, height); //FRONT
		mesh->m_Vertices[21].a_Position = vec3(0.f, 0.f, 0.f);
		mesh->m_Vertices[22].a_Position = vec3(width, 0.f, 0.f);
		mesh->m_Vertices[23].a_Position = vec3(width, 0.f, height);
		mesh->m_Vertices[20].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[21].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[22].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[23].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[20].a_Colour = vec4(1.f, 0.f, 1.f, 1.f); //PURPLE
		mesh->m_Vertices[21].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[22].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[23].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		//mesh->m_Vertices[20].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //PURPLE
		//mesh->m_Vertices[21].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[22].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[23].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[20].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[21].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[22].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[23].a_UV = vec2(1.f, 0.f);

		mesh->m_Indices[0] = 0; //BOTTOM
		mesh->m_Indices[1] = 1;
		mesh->m_Indices[2] = 3;
		mesh->m_Indices[3] = 3;
		mesh->m_Indices[4] = 1;
		mesh->m_Indices[5] = 2;

		mesh->m_Indices[6] = 4; //TOP
		mesh->m_Indices[7] = 5;
		mesh->m_Indices[8] = 7;
		mesh->m_Indices[9] = 7;
		mesh->m_Indices[10] = 5;
		mesh->m_Indices[11] = 6;

		mesh->m_Indices[12] = 8; //LEFT
		mesh->m_Indices[13] = 9;
		mesh->m_Indices[14] = 11;
		mesh->m_Indices[15] = 11;
		mesh->m_Indices[16] = 9;
		mesh->m_Indices[17] = 10;

		mesh->m_Indices[18] = 12; //RIGHT
		mesh->m_Indices[19] = 13;
		mesh->m_Indices[20] = 15;
		mesh->m_Indices[21] = 15;
		mesh->m_Indices[22] = 13;
		mesh->m_Indices[23] = 14;

		mesh->m_Indices[24] = 16; //BACK
		mesh->m_Indices[25] = 17;
		mesh->m_Indices[26] = 19;
		mesh->m_Indices[27] = 19;
		mesh->m_Indices[28] = 17;
		mesh->m_Indices[29] = 18;

		mesh->m_Indices[30] = 20; //FRONT
		mesh->m_Indices[31] = 21;
		mesh->m_Indices[32] = 23;
		mesh->m_Indices[33] = 23;
		mesh->m_Indices[34] = 21;
		mesh->m_Indices[35] = 22;

		return mesh;
	}


	Ref<Mesh> MeshFactory::rectCorner(float width, float height, float depth, glm::vec4 colour)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(24, 36);

		using namespace glm;
		mesh->m_Vertices[0].a_Position = vec3(0.f, 0.f, 0.f); //BOTTOM
		mesh->m_Vertices[1].a_Position = vec3(0.f, depth, 0.f);
		mesh->m_Vertices[2].a_Position = vec3(width, depth, 0.f);
		mesh->m_Vertices[3].a_Position = vec3(width, 0.f, 0.f);
		mesh->m_Vertices[0].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[1].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[2].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[3].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[0].a_Colour = colour;
		mesh->m_Vertices[1].a_Colour = colour;
		mesh->m_Vertices[2].a_Colour = colour;
		mesh->m_Vertices[3].a_Colour = colour;
		mesh->m_Vertices[0].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[1].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[2].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[3].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[4].a_Position = vec3(0.f, depth, height); //TOP
		mesh->m_Vertices[5].a_Position = vec3(0.f, 0.f, height);
		mesh->m_Vertices[6].a_Position = vec3(width, 0.f, height);
		mesh->m_Vertices[7].a_Position = vec3(width, depth, height);
		mesh->m_Vertices[4].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[5].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[6].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[7].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[4].a_Colour = colour;
		mesh->m_Vertices[5].a_Colour = colour;
		mesh->m_Vertices[6].a_Colour = colour;
		mesh->m_Vertices[7].a_Colour = colour;
		mesh->m_Vertices[4].a_UV = vec2(0.01f, 0.01f);
		mesh->m_Vertices[5].a_UV = vec2(0.01f, 0.99f);
		mesh->m_Vertices[6].a_UV = vec2(0.99f, 0.99f);
		mesh->m_Vertices[7].a_UV = vec2(0.99f, 0.01f);

		mesh->m_Vertices[8].a_Position = vec3(0.f, 0.f, height); //LEFT
		mesh->m_Vertices[9].a_Position = vec3(0.f, depth, height);
		mesh->m_Vertices[10].a_Position = vec3(0.f, depth, 0.f);
		mesh->m_Vertices[11].a_Position = vec3(0.f, 0.f, 0.f);
		mesh->m_Vertices[8].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[9].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[10].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[11].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[8 ].a_Colour = colour;
		mesh->m_Vertices[9 ].a_Colour = colour;
		mesh->m_Vertices[10].a_Colour = colour;
		mesh->m_Vertices[11].a_Colour = colour;
		mesh->m_Vertices[8].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[9].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[10].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[11].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[12].a_Position = vec3(width, 0.f, 0.f); //RIGHT
		mesh->m_Vertices[13].a_Position = vec3(width, depth, 0.f);
		mesh->m_Vertices[14].a_Position = vec3(width, depth, height);
		mesh->m_Vertices[15].a_Position = vec3(width, 0.f, height);
		mesh->m_Vertices[12].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[13].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[14].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[15].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[12].a_Colour = colour;
		mesh->m_Vertices[13].a_Colour = colour;
		mesh->m_Vertices[14].a_Colour = colour;
		mesh->m_Vertices[15].a_Colour = colour;
		mesh->m_Vertices[12].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[13].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[14].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[15].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[16].a_Position = vec3(0.f, depth, 0.f); //BACK
		mesh->m_Vertices[17].a_Position = vec3(0.f, depth, height);
		mesh->m_Vertices[18].a_Position = vec3(width, depth, height);
		mesh->m_Vertices[19].a_Position = vec3(width, depth, 0.f);
		mesh->m_Vertices[16].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[17].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[18].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[19].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[16].a_Colour = colour;
		mesh->m_Vertices[17].a_Colour = colour;
		mesh->m_Vertices[18].a_Colour = colour;
		mesh->m_Vertices[19].a_Colour = colour;
		mesh->m_Vertices[16].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[17].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[18].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[19].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[20].a_Position = vec3(0.f, 0.f, height); //FRONT
		mesh->m_Vertices[21].a_Position = vec3(0.f, 0.f, 0.f);
		mesh->m_Vertices[22].a_Position = vec3(width, 0.f, 0.f);
		mesh->m_Vertices[23].a_Position = vec3(width, 0.f, height);
		mesh->m_Vertices[20].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[21].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[22].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[23].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[20].a_Colour = colour;
		mesh->m_Vertices[21].a_Colour = colour;
		mesh->m_Vertices[22].a_Colour = colour;
		mesh->m_Vertices[23].a_Colour = colour;
		mesh->m_Vertices[20].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[21].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[22].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[23].a_UV = vec2(1.f, 0.f);

		mesh->m_Indices[0] = 0; //BOTTOM
		mesh->m_Indices[1] = 1;
		mesh->m_Indices[2] = 3;
		mesh->m_Indices[3] = 3;
		mesh->m_Indices[4] = 1;
		mesh->m_Indices[5] = 2;

		mesh->m_Indices[6] = 4; //TOP
		mesh->m_Indices[7] = 5;
		mesh->m_Indices[8] = 7;
		mesh->m_Indices[9] = 7;
		mesh->m_Indices[10] = 5;
		mesh->m_Indices[11] = 6;

		mesh->m_Indices[12] = 8; //LEFT
		mesh->m_Indices[13] = 9;
		mesh->m_Indices[14] = 11;
		mesh->m_Indices[15] = 11;
		mesh->m_Indices[16] = 9;
		mesh->m_Indices[17] = 10;

		mesh->m_Indices[18] = 12; //RIGHT
		mesh->m_Indices[19] = 13;
		mesh->m_Indices[20] = 15;
		mesh->m_Indices[21] = 15;
		mesh->m_Indices[22] = 13;
		mesh->m_Indices[23] = 14;

		mesh->m_Indices[24] = 16; //BACK
		mesh->m_Indices[25] = 17;
		mesh->m_Indices[26] = 19;
		mesh->m_Indices[27] = 19;
		mesh->m_Indices[28] = 17;
		mesh->m_Indices[29] = 18;

		mesh->m_Indices[30] = 20; //FRONT
		mesh->m_Indices[31] = 21;
		mesh->m_Indices[32] = 23;
		mesh->m_Indices[33] = 23;
		mesh->m_Indices[34] = 21;
		mesh->m_Indices[35] = 22;

		return mesh;
	}

	Ref<Mesh> MeshFactory::planeCenter(glm::vec3 x, glm::vec3 y)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(4, 6);

		glm::vec3 normal = glm::normalize(glm::cross(x, y));

		using namespace glm;
		mesh->m_Vertices[0].a_Position = vec3(-0.5f * (x.x + y.x), -0.5f * (x.y + y.y), -0.5f * (x.z + y.z));
		mesh->m_Vertices[1].a_Position = vec3(+0.5f * (x.x - y.x), +0.5f * (x.y - y.y), +0.5f * (x.z - y.z));
		mesh->m_Vertices[2].a_Position = vec3(+0.5f * (x.x + y.x), +0.5f * (x.y + y.y), +0.5f * (x.z + y.z));
		mesh->m_Vertices[3].a_Position = vec3(-0.5f * (x.x - y.x), -0.5f * (x.y - y.y), -0.5f * (x.z - y.z));
		mesh->m_Vertices[0].a_Normal = normal;
		mesh->m_Vertices[1].a_Normal = normal;
		mesh->m_Vertices[2].a_Normal = normal;
		mesh->m_Vertices[3].a_Normal = normal;
		mesh->m_Vertices[0].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[1].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[2].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[3].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[0].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[1].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[2].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[3].a_UV = vec2(1.f, 0.f);

		mesh->m_Indices[0] = 0;
		mesh->m_Indices[1] = 1;
		mesh->m_Indices[2] = 3;
		mesh->m_Indices[3] = 3;
		mesh->m_Indices[4] = 1;
		mesh->m_Indices[5] = 2;

		return mesh;
	}

	Ref<Mesh> MeshFactory::planeCorner(glm::vec3 i, glm::vec3 j)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(4, 6);

		glm::vec3 normal = glm::normalize(glm::cross(i, j));

		using namespace glm;
		mesh->m_Vertices[0].a_Position = vec3(0.f, 0.f, 0.f);
		mesh->m_Vertices[1].a_Position = vec3(i.x, i.y, i.z);
		mesh->m_Vertices[2].a_Position = vec3(i.x + j.x, i.y + j.y, i.z + j.z);
		mesh->m_Vertices[3].a_Position = vec3(j.x, j.y, j.z);
		mesh->m_Vertices[0].a_Normal = normal;
		mesh->m_Vertices[1].a_Normal = normal;
		mesh->m_Vertices[2].a_Normal = normal;
		mesh->m_Vertices[3].a_Normal = normal;
		mesh->m_Vertices[0].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[1].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[2].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[3].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[0].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[1].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[2].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[3].a_UV = vec2(1.f, 0.f);

		mesh->m_Indices[0] = 0;
		mesh->m_Indices[1] = 1;
		mesh->m_Indices[2] = 3;
		mesh->m_Indices[3] = 3;
		mesh->m_Indices[4] = 1;
		mesh->m_Indices[5] = 2;

		return mesh;
	}

	Ref<Mesh> MeshFactory::gridCenter(glm::vec3 xdir, glm::vec3 ydir, int xdetail, int ydetail, glm::vec4 colour)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(xdetail * ydetail, (xdetail - 1) * (ydetail - 1) * 6);

		using namespace glm;

		for (int y = 0; y < ydetail; y++)
		{
			vec3 v = ((float)y / (ydetail - 1) - 0.5f) * ydir;
			for (int x = 0; x < xdetail; x++)
			{
				vec3 u = ((float)x / (xdetail - 1) - 0.5f) * xdir;

				auto& vert = mesh->m_Vertices[x + y * xdetail];
				vert.a_Position = u + v;
				vert.a_Colour = colour;
				vert.a_Normal = glm::normalize(glm::cross(xdir, ydir));
				vert.a_Tex = -1.f;
				vert.a_UV = glm::vec2((float)x / (xdetail - 1.f), (float)y / (ydetail - 1.f));
			}
		}

		for (int v = 0; v < ydetail - 1; v++)
		{
			for (int u = 0; u < xdetail - 1; u++)
			{
				for (int n = 0; n < 6; n++)
				{
					glm::ivec2 vert = tvertices[tindices[n]];
					mesh->m_Indices[(u + v * (xdetail - 1)) * 6 + n] = (vert.x + u) + (vert.y + v) * xdetail;
				}
			}
		}

		return mesh;
	}

	Ref<Mesh> MeshFactory::gridCorner(glm::vec3 xdir, glm::vec3 ydir, int xdetail, int ydetail, glm::vec4 colour)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(xdetail * ydetail, (xdetail - 1) * (ydetail - 1) * 6);

		using namespace glm;

		for (int y = 0; y < ydetail; y++)
		{
			vec3 v = (float)y / (ydetail - 1) * ydir;
			for (int x = 0; x < xdetail; x++)
			{
				vec3 u = (float)x / (xdetail - 1) * xdir;

				auto& vert = mesh->m_Vertices[x + y * xdetail];
				vert.a_Position = u + v;
				vert.a_Colour = colour;
				vert.a_Normal = glm::normalize(glm::cross(u, v));
				vert.a_Tex = -1.f;
				vert.a_UV = glm::vec2((float)x / (xdetail - 1.f), (float)y / (ydetail - 1.f));
			}
		}

		for (int v = 0; v < ydetail - 1; v++)
		{
			for (int u = 0; u < xdetail - 1; u++)
			{
				for (int n = 0; n < 6; n++)
				{
					glm::ivec2 vert = tvertices[tindices[n]];
					mesh->m_Indices[(u + v * (xdetail - 1)) * 6 + n] = (vert.x + u) + (vert.y + v) * xdetail;
				}
			}
		}

		return mesh;
	}

	Ref<Mesh> MeshFactory::rectCenterSharedVertices(float width, float height, float depth)
	{
		float depthover2 = depth / 2.f;
		float widthover2 = width / 2.f;
		float heightover2 = height / 2.f;

		Ref<Mesh> mesh = MakeRef<Mesh>(8, 36);

		float c = sqrtf(3.f) / 3.f;

		using namespace glm;
		mesh->m_Vertices[0].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[1].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[2].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[3].a_Position = vec3(+widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[4].a_Position = vec3(-widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[5].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[6].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[7].a_Position = vec3(+widthover2, -depthover2, +heightover2);

		mesh->m_Vertices[0].a_Normal = vec3(-c, -c, -c);
		mesh->m_Vertices[1].a_Normal = vec3(-c, +c, -c);
		mesh->m_Vertices[2].a_Normal = vec3(+c, +c, -c);
		mesh->m_Vertices[3].a_Normal = vec3(+c, -c, -c);
		mesh->m_Vertices[4].a_Normal = vec3(-c, -c, +c);
		mesh->m_Vertices[5].a_Normal = vec3(-c, +c, +c);
		mesh->m_Vertices[6].a_Normal = vec3(+c, +c, +c);
		mesh->m_Vertices[7].a_Normal = vec3(+c, -c, +c);

		mesh->m_Vertices[0].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[1].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[2].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[3].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[4].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[5].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[6].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[7].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);

		mesh->m_Indices[0] = 0;
		mesh->m_Indices[1] = 1;
		mesh->m_Indices[2] = 3;
		mesh->m_Indices[3] = 3;
		mesh->m_Indices[4] = 1;
		mesh->m_Indices[5] = 2;

		mesh->m_Indices[6] = 3;
		mesh->m_Indices[7] = 2;
		mesh->m_Indices[8] = 7;
		mesh->m_Indices[9] = 7;
		mesh->m_Indices[10] = 2;
		mesh->m_Indices[11] = 6;

		mesh->m_Indices[12] = 7;
		mesh->m_Indices[13] = 6;
		mesh->m_Indices[14] = 4;
		mesh->m_Indices[15] = 4;
		mesh->m_Indices[16] = 6;
		mesh->m_Indices[17] = 5;

		mesh->m_Indices[18] = 4;
		mesh->m_Indices[19] = 5;
		mesh->m_Indices[20] = 0;
		mesh->m_Indices[21] = 0;
		mesh->m_Indices[22] = 5;
		mesh->m_Indices[23] = 1;

		mesh->m_Indices[24] = 1;
		mesh->m_Indices[25] = 5;
		mesh->m_Indices[26] = 2;
		mesh->m_Indices[27] = 2;
		mesh->m_Indices[28] = 5;
		mesh->m_Indices[29] = 6;

		mesh->m_Indices[30] = 7;
		mesh->m_Indices[31] = 4;
		mesh->m_Indices[32] = 3;
		mesh->m_Indices[33] = 3;
		mesh->m_Indices[34] = 4;
		mesh->m_Indices[35] = 0;

		return mesh;
	}

	Ref<Mesh> MeshFactory::rectCenter(float width, float height, float depth, glm::vec4 colour)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(24, 36);

		float depthover2 = depth / 2.f;
		float widthover2 = width / 2.f;
		float heightover2 = height / 2.f;
	
		using namespace glm;
		mesh->m_Vertices[0 ].a_Position = vec3(-widthover2, -depthover2, -heightover2); //BOTTOM
		mesh->m_Vertices[1 ].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[2 ].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[3 ].a_Position = vec3(+widthover2, -depthover2, -heightover2);								
		mesh->m_Vertices[0 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		mesh->m_Vertices[1 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		mesh->m_Vertices[2 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		mesh->m_Vertices[3 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		mesh->m_Vertices[0 ].a_Colour = colour;
		mesh->m_Vertices[1 ].a_Colour = colour;
		mesh->m_Vertices[2 ].a_Colour = colour;
		mesh->m_Vertices[3 ].a_Colour = colour;
		mesh->m_Vertices[0 ].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[1 ].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[2 ].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[3 ].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[4 ].a_Position = vec3(-widthover2, +depthover2, +heightover2); //TOP
		mesh->m_Vertices[5 ].a_Position = vec3(-widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[6 ].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[7 ].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[4 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		mesh->m_Vertices[5 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		mesh->m_Vertices[6 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		mesh->m_Vertices[7 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		mesh->m_Vertices[4 ].a_Colour = colour;
		mesh->m_Vertices[5 ].a_Colour = colour;
		mesh->m_Vertices[6 ].a_Colour = colour;
		mesh->m_Vertices[7 ].a_Colour = colour;
		mesh->m_Vertices[4 ].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[5 ].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[6 ].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[7 ].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[8 ].a_Position = vec3(-widthover2, -depthover2, +heightover2); //LEFT
		mesh->m_Vertices[9 ].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[10].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[11].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[8 ].a_Normal = vec3(-1.f,  0.f,  0.f);
		mesh->m_Vertices[9 ].a_Normal = vec3(-1.f,  0.f,  0.f);
		mesh->m_Vertices[10].a_Normal = vec3(-1.f,  0.f,  0.f);
		mesh->m_Vertices[11].a_Normal = vec3(-1.f,  0.f,  0.f);
		mesh->m_Vertices[8 ].a_Colour = colour;
		mesh->m_Vertices[9 ].a_Colour = colour;
		mesh->m_Vertices[10].a_Colour = colour;
		mesh->m_Vertices[11].a_Colour = colour;
		mesh->m_Vertices[8 ].a_UV = vec2(0.01f, 0.01f);
		mesh->m_Vertices[9 ].a_UV = vec2(0.01f, 0.99f);
		mesh->m_Vertices[10].a_UV = vec2(0.99f, 0.99f);
		mesh->m_Vertices[11].a_UV = vec2(0.99f, 0.01f);

		mesh->m_Vertices[12].a_Position = vec3(+widthover2, -depthover2, -heightover2); //RIGHT
		mesh->m_Vertices[13].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[14].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[15].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[12].a_Normal = vec3(+1.f,  0.f,  0.f);
		mesh->m_Vertices[13].a_Normal = vec3(+1.f,  0.f,  0.f);
		mesh->m_Vertices[14].a_Normal = vec3(+1.f,  0.f,  0.f);
		mesh->m_Vertices[15].a_Normal = vec3(+1.f,  0.f,  0.f);
		mesh->m_Vertices[12].a_Colour = colour;
		mesh->m_Vertices[13].a_Colour = colour;
		mesh->m_Vertices[14].a_Colour = colour;
		mesh->m_Vertices[15].a_Colour = colour;
		mesh->m_Vertices[12].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[13].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[14].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[15].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[16].a_Position = vec3(-widthover2, +depthover2, -heightover2); //BACK
		mesh->m_Vertices[17].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[18].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[19].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[16].a_Normal = vec3( 0.f, +1.f,  0.f);
		mesh->m_Vertices[17].a_Normal = vec3( 0.f, +1.f,  0.f);
		mesh->m_Vertices[18].a_Normal = vec3( 0.f, +1.f,  0.f);
		mesh->m_Vertices[19].a_Normal = vec3( 0.f, +1.f,  0.f);
		mesh->m_Vertices[16].a_Colour = colour;
		mesh->m_Vertices[17].a_Colour = colour;
		mesh->m_Vertices[18].a_Colour = colour;
		mesh->m_Vertices[19].a_Colour = colour;
		mesh->m_Vertices[16].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[17].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[18].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[19].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[20].a_Position = vec3(-widthover2, -depthover2, +heightover2); //FRONT
		mesh->m_Vertices[21].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[22].a_Position = vec3(+widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[23].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[20].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[21].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[22].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[23].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[20].a_Colour = colour;
		mesh->m_Vertices[21].a_Colour = colour;
		mesh->m_Vertices[22].a_Colour = colour;
		mesh->m_Vertices[23].a_Colour = colour;
		mesh->m_Vertices[20].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[21].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[22].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[23].a_UV = vec2(1.f, 0.f);

		mesh->m_Indices[0 ] = 0; //BOTTOM
		mesh->m_Indices[1 ] = 1;
		mesh->m_Indices[2 ] = 3;
		mesh->m_Indices[3 ] = 3;
		mesh->m_Indices[4 ] = 1;
		mesh->m_Indices[5 ] = 2;

		mesh->m_Indices[6 ] = 4; //TOP
		mesh->m_Indices[7 ] = 5;
		mesh->m_Indices[8 ] = 7;
		mesh->m_Indices[9 ] = 7;
		mesh->m_Indices[10] = 5;
		mesh->m_Indices[11] = 6;

		mesh->m_Indices[12] = 8; //LEFT
		mesh->m_Indices[13] = 9;
		mesh->m_Indices[14] = 11;
		mesh->m_Indices[15] = 11;
		mesh->m_Indices[16] = 9;
		mesh->m_Indices[17] = 10;

		mesh->m_Indices[18] = 12; //RIGHT
		mesh->m_Indices[19] = 13;
		mesh->m_Indices[20] = 15;
		mesh->m_Indices[21] = 15;
		mesh->m_Indices[22] = 13;
		mesh->m_Indices[23] = 14;

		mesh->m_Indices[24] = 16; //BACK
		mesh->m_Indices[25] = 17;
		mesh->m_Indices[26] = 19;
		mesh->m_Indices[27] = 19;
		mesh->m_Indices[28] = 17;
		mesh->m_Indices[29] = 18;

		mesh->m_Indices[30] = 20; //FRONT
		mesh->m_Indices[31] = 21;
		mesh->m_Indices[32] = 23;
		mesh->m_Indices[33] = 23;
		mesh->m_Indices[34] = 21;
		mesh->m_Indices[35] = 22;

		return mesh;
	}

	Ref<Mesh> MeshFactory::griddedCubeCenter(float size, int detail, glm::vec4 colour, std::vector<int>* strides)
	{
		return griddedRectCenter(size, size, size, detail, detail, detail, colour, strides);
	}

	Ref<Mesh> MeshFactory::griddedRectCenter(float width, float height, float depth, int wdetail, int hdetail, int ddetail, glm::vec4 colour, std::vector<int>* strides)
	{
		using namespace glm;

		float wd = (float)(wdetail - 3) / (float)(wdetail - 1);
		float hd = (float)(hdetail - 3) / (float)(hdetail - 1);

		Ref<Mesh> bottom = gridCenter(vec3(-width, 0.f, 0.f), vec3(0.f, depth, 0.f), wdetail, ddetail, colour);
		Ref<Mesh> top	 = gridCenter(vec3(width, 0.f, 0.f), vec3(0.f, depth, 0.f), wdetail, ddetail, colour);

		Ref<Mesh> right	 = gridCenter(vec3(0.f, depth, 0.f), vec3(0.f, 0.f, height * hd), ddetail, hdetail - 2, colour);
		Ref<Mesh> left	 = gridCenter(vec3(0.f, -depth, 0.f), vec3(0.f, 0.f, height * hd), ddetail, hdetail - 2, colour);

		Ref<Mesh> back	 = gridCenter(vec3(-width * wd, 0.f, 0.f), vec3(0.f, 0.f, height * hd), wdetail - 2, hdetail - 2, colour);
		Ref<Mesh> front	 = gridCenter(vec3(width * wd, 0.f, 0.f), vec3(0.f, 0.f, height * hd), wdetail - 2, hdetail - 2, colour);


		bottom->Transform(glm::translate(mat4(1.f), vec3(0.f, 0.f, -height / 2.f)));
		top->Transform(glm::translate(mat4(1.f), vec3(0.f, 0.f, height / 2.f)));
		right->Transform(glm::translate(mat4(1.f), vec3(width / 2.f, 0.f, 0.f)));
		left->Transform(glm::translate(mat4(1.f), vec3(-width / 2.f, 0.f, 0.f)));
		back->Transform(glm::translate(mat4(1.f), vec3(0.f, depth / 2.f, 0.f)));
		front->Transform(glm::translate(mat4(1.f), vec3(0.f, -depth / 2.f, 0.f)));

		int vertoffs = 0;
		int bottomvertoffs = vertoffs;
		int topvertoffs = vertoffs += bottom->m_VerticesCount;
		int rightvertoffs = vertoffs += top->m_VerticesCount;
		int leftvertoffs = vertoffs += right->m_VerticesCount;
		int backvertoffs = vertoffs += left->m_VerticesCount;
		int frontvertoffs = vertoffs += back->m_VerticesCount;
		int indeoffs = 0;

		if (strides)
		{
			strides->push_back(bottomvertoffs);
			strides->push_back(topvertoffs);
			strides->push_back(rightvertoffs);
			strides->push_back(leftvertoffs);
			strides->push_back(backvertoffs);
			strides->push_back(frontvertoffs);
		}

		Ref<Mesh> gaps = MakeRef<Mesh>(0, (ddetail - 1) * 6 * 4 + (wdetail - 1) * 6 * 4 + (hdetail - 2 - 1) * 6 * 4);
		int gindeoffs = 0;

		//for (int i = 0; i < (ddetail - 1) * 6; i++) gaps->m_Indices[gindeoffs++] = ;

		gaps->m_Indices[gindeoffs++] = bottomvertoffs + wdetail - 2;
		gaps->m_Indices[gindeoffs++] = leftvertoffs + ddetail - 1;
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + wdetail - 1; //FRONT BOTTOM LEFT
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + wdetail - 2;
		gaps->m_Indices[gindeoffs++] = frontvertoffs;
		gaps->m_Indices[gindeoffs++] = leftvertoffs + ddetail - 1;

		gaps->m_Indices[gindeoffs++] = bottomvertoffs;
		gaps->m_Indices[gindeoffs++] = rightvertoffs;
		gaps->m_Indices[gindeoffs++] = frontvertoffs + wdetail - 3; // FRONT BOTTOM RIGHT
		gaps->m_Indices[gindeoffs++] = bottomvertoffs;
		gaps->m_Indices[gindeoffs++] = frontvertoffs + wdetail - 3;
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + 1;

		gaps->m_Indices[gindeoffs++] = frontvertoffs + (hdetail - 3) * (wdetail - 2); // FRONT TOP LEFT
		gaps->m_Indices[gindeoffs++] = topvertoffs;
		gaps->m_Indices[gindeoffs++] = leftvertoffs + ddetail - 1 + (hdetail - 3) * ddetail;
		gaps->m_Indices[gindeoffs++] = frontvertoffs + (hdetail - 3) * (wdetail - 2);
		gaps->m_Indices[gindeoffs++] = topvertoffs + 1;
		gaps->m_Indices[gindeoffs++] = topvertoffs;

		gaps->m_Indices[gindeoffs++] = rightvertoffs + (hdetail - 3) * ddetail; // FRONT TOP RIGHT
		gaps->m_Indices[gindeoffs++] = topvertoffs + wdetail - 1;
		gaps->m_Indices[gindeoffs++] = topvertoffs + wdetail - 2;
		gaps->m_Indices[gindeoffs++] = rightvertoffs + (hdetail - 3) * ddetail;
		gaps->m_Indices[gindeoffs++] = topvertoffs + wdetail - 2;
		gaps->m_Indices[gindeoffs++] = frontvertoffs + +(hdetail - 3) * (wdetail - 2) + wdetail - 3;

		for (int i = 0; i < wdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = y * (frontvertoffs + i + x) + (1 - y) * (wdetail - (bottomvertoffs + i + 1 + x) - 1);
			}

		for (int i = 0; i < wdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = (1 - y) * (frontvertoffs + i + x + (hdetail - 3) * (wdetail - 2)) + y * (topvertoffs + i + 1 + x);
			}

		for (int i = 0; i < hdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = x * (frontvertoffs + (i + y) * (wdetail - 2)) + (1 - x) * (leftvertoffs + (i + y) * ddetail + ddetail - 1);
			}
		
		for (int i = 0; i < hdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = (1 - x) * (frontvertoffs + (i + y) * (wdetail - 2) + wdetail - 3) + x * (rightvertoffs + (i + y) * ddetail);
			}

		
		for (int i = 0; i < ddetail - 1; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = (1 - y) * (leftvertoffs + ddetail - (i + x) - 1) + y * (bottomvertoffs + (i + x) * wdetail + wdetail - 1);
			}

		for (int i = 0; i < ddetail - 1; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = y * (leftvertoffs + ddetail - (i + x) - 1 + (hdetail - 3) * ddetail) + (1 - y) * (topvertoffs + (i + x) * wdetail);
			}

		for (int i = 0; i < ddetail - 1; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = y * (rightvertoffs + i + x) + (1 - y) * (bottomvertoffs + (i + x) * wdetail);
			}

		for (int i = 0; i < ddetail - 1; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = (1 - y) * (rightvertoffs + i + x + (hdetail - 3) * ddetail) + y * (topvertoffs + (i + x) * wdetail + wdetail - 1);
			}

		
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + wdetail - 1 + (ddetail - 1) * wdetail; //BACK BOTTOM LEFT
		gaps->m_Indices[gindeoffs++] = backvertoffs + wdetail - 3;
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + wdetail - 2 + (ddetail - 1) * wdetail;
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + wdetail - 1 + (ddetail - 1) * wdetail;
		gaps->m_Indices[gindeoffs++] = leftvertoffs;
		gaps->m_Indices[gindeoffs++] = backvertoffs + wdetail - 3;

		gaps->m_Indices[gindeoffs++] = bottomvertoffs + (ddetail - 1) * wdetail; //FRONT BOTTOM RIGHT
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + 1 + (ddetail - 1) * wdetail;
		gaps->m_Indices[gindeoffs++] = rightvertoffs + ddetail - 1;
		gaps->m_Indices[gindeoffs++] = bottomvertoffs + 1 + (ddetail - 1) * wdetail;
		gaps->m_Indices[gindeoffs++] = backvertoffs;
		gaps->m_Indices[gindeoffs++] = rightvertoffs + ddetail - 1;

		gaps->m_Indices[gindeoffs++] = backvertoffs + (hdetail - 3) * (wdetail - 2);
		gaps->m_Indices[gindeoffs++] = topvertoffs + + wdetail - 1 + (ddetail - 1) * wdetail;
		gaps->m_Indices[gindeoffs++] = rightvertoffs + (hdetail - 3) * ddetail +  ddetail - 1;
		gaps->m_Indices[gindeoffs++] = backvertoffs + (hdetail - 3) * (wdetail - 2);
		gaps->m_Indices[gindeoffs++] = topvertoffs + wdetail - 2 + (ddetail - 1) * wdetail;
		gaps->m_Indices[gindeoffs++] = topvertoffs + + wdetail - 1 + (ddetail - 1) * wdetail; //BACK TOP LEFT

		gaps->m_Indices[gindeoffs++] = backvertoffs + (hdetail - 3) * (wdetail - 2) + wdetail - 3;
		gaps->m_Indices[gindeoffs++] = leftvertoffs + (hdetail - 3) * ddetail;
		gaps->m_Indices[gindeoffs++] = topvertoffs + (ddetail - 1) * wdetail + 1;
		gaps->m_Indices[gindeoffs++] = leftvertoffs + (hdetail - 3) * ddetail;
		gaps->m_Indices[gindeoffs++] = topvertoffs + (ddetail - 1) * wdetail;
		gaps->m_Indices[gindeoffs++] = topvertoffs + (ddetail - 1) * wdetail + 1;

		

		for (int i = 0; i < wdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = y * (backvertoffs + i + x) + (1 - y) * (bottomvertoffs + i + 1 + x + wdetail * (ddetail - 1));
			}
		
		for (int i = 0; i < wdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = (1 - y) * (backvertoffs + i + x + (hdetail - 3) * (wdetail - 2)) + y * (topvertoffs + wdetail - (i + 1 + x) - 1 + wdetail * (ddetail - 1));
			}
		
		for (int i = 0; i < hdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = x * (backvertoffs + (i + y) * (wdetail - 2)) + (1 - x) * (rightvertoffs + (i + y) * ddetail + ddetail - 1);
			}
		
		for (int i = 0; i < hdetail - 3; i++)
			for (int n = 0; n < 6; n++)
			{
				const int& x = tvertices[tindices[n]].x;
				const int& y = tvertices[tindices[n]].y;
				gaps->m_Indices[gindeoffs++] = (1 - x) * (backvertoffs + (i + y) * (wdetail - 2) + wdetail - 3) + x * (leftvertoffs + (i + y) * ddetail);
			}
			
		Ref<Mesh> mesh = MakeRef<Mesh>(wdetail * ddetail * 2 + ddetail * (hdetail - 2) * 2 + (wdetail - 2) * (hdetail - 2) * 2, (wdetail - 1) * (ddetail - 1) * 12 + (hdetail - 1) * (ddetail - 1) * 12 + (wdetail - 1) * (hdetail - 1) * 12);
		mesh->Insert(bottom, bottomvertoffs, 0);
		mesh->Insert(top	, topvertoffs, indeoffs += bottom->m_IndicesCount);
		mesh->Insert(right	, rightvertoffs, indeoffs += top->m_IndicesCount);
		mesh->Insert(left	, leftvertoffs, indeoffs += right->m_IndicesCount);
		mesh->Insert(back	, backvertoffs, indeoffs += left->m_IndicesCount);
		mesh->Insert(front	, frontvertoffs, indeoffs += back->m_IndicesCount);

		mesh->Insert(gaps, 0, indeoffs += front->m_IndicesCount);

		return mesh;
	}

	Ref<Mesh> MeshFactory::colourRectCenter(float width, float height, float depth)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(24, 36);

		float depthover2 = depth / 2.f;
		float widthover2 = width / 2.f;
		float heightover2 = height / 2.f;

		using namespace glm;
		mesh->m_Vertices[0].a_Position = vec3(-widthover2, -depthover2, -heightover2); //BOTTOM
		mesh->m_Vertices[1].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[2].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[3].a_Position = vec3(+widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[0].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[1].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[2].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[3].a_Normal = vec3(0.f, 0.f, -1.f);
		mesh->m_Vertices[0].a_Colour = vec4(1.f, 0.f, 0.f, 1.f); //RED
		mesh->m_Vertices[1].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[2].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[3].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[0].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[1].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[2].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[3].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[4].a_Position = vec3(-widthover2, +depthover2, +heightover2); //TOP
		mesh->m_Vertices[5].a_Position = vec3(-widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[6].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[7].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[4].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[5].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[6].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[7].a_Normal = vec3(0.f, 0.f, +1.f);
		mesh->m_Vertices[4].a_Colour = vec4(0.f, 1.f, 0.f, 1.f); //GREEN
		mesh->m_Vertices[5].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[6].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[7].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[4].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[5].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[6].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[7].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[8].a_Position = vec3(-widthover2, -depthover2, +heightover2); //LEFT
		mesh->m_Vertices[9].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[10].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[11].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[8].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[9].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[10].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[11].a_Normal = vec3(-1.f, 0.f, 0.f);
		mesh->m_Vertices[8].a_Colour = vec4(0.f, 0.f, 1.f, 1.f); //BLUE
		mesh->m_Vertices[9].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[10].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[11].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[8].a_UV = vec2(0.01f, 0.01f);
		mesh->m_Vertices[9].a_UV = vec2(0.01f, 0.99f);
		mesh->m_Vertices[10].a_UV = vec2(0.99f, 0.99f);
		mesh->m_Vertices[11].a_UV = vec2(0.99f, 0.01f);

		mesh->m_Vertices[12].a_Position = vec3(+widthover2, -depthover2, -heightover2); //RIGHT
		mesh->m_Vertices[13].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[14].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[15].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[12].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[13].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[14].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[15].a_Normal = vec3(+1.f, 0.f, 0.f);
		mesh->m_Vertices[12].a_Colour = vec4(1.f, 1.f, 0.f, 1.f); //YELLOW
		mesh->m_Vertices[13].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[14].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[15].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[12].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[13].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[14].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[15].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[16].a_Position = vec3(-widthover2, +depthover2, -heightover2); //BACK
		mesh->m_Vertices[17].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[18].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[19].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[16].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[17].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[18].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[19].a_Normal = vec3(0.f, +1.f, 0.f);
		mesh->m_Vertices[16].a_Colour = vec4(0.f, 1.f, 1.f, 1.f); //AQUA
		mesh->m_Vertices[17].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[18].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[19].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[16].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[17].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[18].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[19].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[20].a_Position = vec3(-widthover2, -depthover2, +heightover2); //FRONT
		mesh->m_Vertices[21].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[22].a_Position = vec3(+widthover2, -depthover2, -heightover2);
		mesh->m_Vertices[23].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[20].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[21].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[22].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[23].a_Normal = vec3(0.f, -1.f, 0.f);
		mesh->m_Vertices[20].a_Colour = vec4(1.f, 0.f, 1.f, 1.f); //PURPLE
		mesh->m_Vertices[21].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[22].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[23].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[20].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[21].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[22].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[23].a_UV = vec2(1.f, 0.f);

		mesh->m_Indices[0] = 0; //BOTTOM
		mesh->m_Indices[1] = 1;
		mesh->m_Indices[2] = 3;
		mesh->m_Indices[3] = 3;
		mesh->m_Indices[4] = 1;
		mesh->m_Indices[5] = 2;

		mesh->m_Indices[6] = 4; //TOP
		mesh->m_Indices[7] = 5;
		mesh->m_Indices[8] = 7;
		mesh->m_Indices[9] = 7;
		mesh->m_Indices[10] = 5;
		mesh->m_Indices[11] = 6;

		mesh->m_Indices[12] = 8; //LEFT
		mesh->m_Indices[13] = 9;
		mesh->m_Indices[14] = 11;
		mesh->m_Indices[15] = 11;
		mesh->m_Indices[16] = 9;
		mesh->m_Indices[17] = 10;

		mesh->m_Indices[18] = 12; //RIGHT
		mesh->m_Indices[19] = 13;
		mesh->m_Indices[20] = 15;
		mesh->m_Indices[21] = 15;
		mesh->m_Indices[22] = 13;
		mesh->m_Indices[23] = 14;

		mesh->m_Indices[24] = 16; //BACK
		mesh->m_Indices[25] = 17;
		mesh->m_Indices[26] = 19;
		mesh->m_Indices[27] = 19;
		mesh->m_Indices[28] = 17;
		mesh->m_Indices[29] = 18;

		mesh->m_Indices[30] = 20; //FRONT
		mesh->m_Indices[31] = 21;
		mesh->m_Indices[32] = 23;
		mesh->m_Indices[33] = 23;
		mesh->m_Indices[34] = 21;
		mesh->m_Indices[35] = 22;

		return mesh;
	}

	Ref<Mesh> MeshFactory::mark()
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(8, 8);

		for (int i = 0; i < 4; i++)
		{
			float theta = i / 4.f * glm::pi<float>();
			float x = cosf(theta) * 0.1f;
			float y = sinf(theta) * 0.1f;

			mesh->m_Vertices[i * 2].a_Position = glm::vec3(x, y, 0.f);
			mesh->m_Vertices[i * 2].a_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
			mesh->m_Vertices[i * 2 + 1].a_Position = glm::vec3(-x, -y, 0.f);
			mesh->m_Vertices[i * 2 + 1].a_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
		}

		for (int i = 0; i < 8; i++)
		{
			mesh->m_Indices[i] = i;
		}

		return mesh;
	}

	Ref<Mesh> MeshFactory::line(const glm::vec3& start, const glm::vec3& end)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(2, 2);

		mesh->m_Vertices[0].a_Position = start;
		mesh->m_Vertices[0].a_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[1].a_Position = end;
		mesh->m_Vertices[1].a_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f);

		mesh->m_Indices[0] = 0;
		mesh->m_Indices[1] = 1;
	}

	Ref<Mesh> MeshFactory::paramatric(parametricfunc_t curvefunc, float tstart, float tend, const int tsteps, parametriccolourfunc_t curvecolourfunc)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(tsteps, 2 * tsteps - 2);

		for (int i = 0; i < tsteps; i++)
		{
			mesh->m_Vertices[i].a_Position = curvefunc(tstart + (tend - tstart) * (float)i / (float)(tsteps - 1));
			mesh->m_Vertices[i].a_Colour = curvecolourfunc(tstart + (tend - tstart) * (float)i / (float)(tsteps - 1));
			mesh->m_Vertices[i].a_Tex = -1.f;
			mesh->m_Vertices[i].a_Normal = glm::vec3(0.f, 0.f, 1.f);
		}

		for (int i = 0; i < tsteps - 1; i++)
		{
			mesh->m_Indices[2 * i] = i;
			mesh->m_Indices[2 * i + 1] = i + 1;
		}

		return mesh;
	}

	Ref<Mesh> MeshFactory::paramatricSurface(surfacefunc_t curvefunc, float ustart, float uend, const int usteps, float vstart, float vend, const int vsteps, surfacecolourfunc_t curvecolourfunc)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(usteps * vsteps, (usteps - 1) * (vsteps - 1) * 6);

		for (int y = 0; y < vsteps; y++)
		{
			float v = (float)y / (vsteps - 1) * (vend - vstart) + vstart;
			for (int x = 0; x < usteps; x++)
			{
				float u = (float)x / (usteps - 1) * (uend - ustart) + ustart;

				auto& vert = mesh->m_Vertices[x + y * usteps];
				auto p = curvefunc(u, v);
				vert.a_Position = p;
				vert.a_Colour = curvecolourfunc(u, v);
				vert.a_Normal = glm::normalize(glm::cross(curvefunc(u + 1.f / 1000.f, v) - p, curvefunc(u, v + 1.f / 1000.f) - p));
				vert.a_Tex = -1.f;
				vert.a_UV = glm::vec2((float)x / (usteps - 1.f), (float)y / (vsteps - 1.f));
			}
		}

		for (int v = 0; v < vsteps - 1; v++)
		{
			for (int u = 0; u < usteps - 1; u++)
			{
				for (int n = 0; n < 6; n++)
				{
					glm::ivec2 vert = tvertices[tindices[n]];
					mesh->m_Indices[(u + v * (usteps - 1)) * 6 + n] = (vert.x + u) + (vert.y + v) * usteps;
				}
			}
		}

		return mesh;
	}
}