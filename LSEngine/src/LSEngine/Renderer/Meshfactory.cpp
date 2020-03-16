#include "lsepch.h"
#include "Meshfactory.h"

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

	Ref<Mesh> MeshFactory::generateSphere(const float& radius, const int& detail, glm::vec4 colour)
	{
		const int width = detail * 2;
		const int height = detail;
		return generateSphere(radius, width, height, colour);
	}

	Ref<Mesh> MeshFactory::generateCubeCorner(float size)
	{
		return MeshFactory::generateRectCorner(size, size, size);
	}

	Ref<Mesh> MeshFactory::generateCubeCenter(float size)
	{
		return MeshFactory::generateRectCenter(size, size, size);
	}

	float a(float f)
	{
		return 1.f - fabsf(f * 2.f - 1.f);
	}

	float b(float f)
	{
		return a(fmodf(2.f * f, 2.f) / 2.f);
	}

	Ref<Mesh> MeshFactory::generateSphere(const float& radius, const int& w, const int& h, glm::vec4 colour)
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


	Ref<Mesh> MeshFactory::generateRectCorner(float width, float height, float depth)
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
		//mesh->m_Vertices[0].a_Colour = vec4(1.f, 0.f, 0.f, 1.f); //RED
		//mesh->m_Vertices[1].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		//mesh->m_Vertices[2].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		//mesh->m_Vertices[3].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[0].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //RED
		mesh->m_Vertices[1].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[2].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[3].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
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
		//mesh->m_Vertices[4].a_Colour = vec4(0.f, 1.f, 0.f, 1.f); //GREEN
		//mesh->m_Vertices[5].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		//mesh->m_Vertices[6].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		//mesh->m_Vertices[7].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[4].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //GREEN
		mesh->m_Vertices[5].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[6].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[7].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[4].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[5].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[6].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[7].a_UV = vec2(1.f, 0.f);

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
		//mesh->m_Vertices[12].a_Colour = vec4(1.f, 1.f, 0.f, 1.f); //YELLOW
		//mesh->m_Vertices[13].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		//mesh->m_Vertices[14].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		//mesh->m_Vertices[15].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[12].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //YELLOW
		mesh->m_Vertices[13].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[14].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[15].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
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
		//mesh->m_Vertices[16].a_Colour = vec4(0.f, 1.f, 1.f, 1.f); //AQUA
		//mesh->m_Vertices[17].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[18].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		//mesh->m_Vertices[19].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[16].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //AQUA
		mesh->m_Vertices[17].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[18].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[19].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
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
		//mesh->m_Vertices[20].a_Colour = vec4(1.f, 0.f, 1.f, 1.f); //PURPLE
		//mesh->m_Vertices[21].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		//mesh->m_Vertices[22].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		//mesh->m_Vertices[23].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[20].a_Colour = vec4(1.f, 1.f, 1.f, 1.f); //PURPLE
		mesh->m_Vertices[21].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[22].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
		mesh->m_Vertices[23].a_Colour = vec4(1.f, 1.f, 1.f, 1.f);
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

	Ref<Mesh> MeshFactory::generatePlaneCorner(glm::vec3 x, glm::vec3 y)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(4, 6);

		glm::vec3 normal = glm::normalize(glm::cross(x, y));

		using namespace glm;
		mesh->m_Vertices[0].a_Position = vec3(0.f, 0.f, 0.f); //BOTTOM
		mesh->m_Vertices[1].a_Position = vec3(x.x, x.y, x.z);
		mesh->m_Vertices[2].a_Position = vec3(x.x + y.x, x.y + y.y, x.z + y.z);
		mesh->m_Vertices[3].a_Position = vec3(y.x, y.y, y.z);
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

		return mesh;
	}

	Ref<Mesh> MeshFactory::generateRectCenterSharedVertices(float width, float height, float depth)
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

	Ref<Mesh> MeshFactory::generateRectCenter(float width, float height, float depth)
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
		mesh->m_Vertices[0 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f); //RED
		mesh->m_Vertices[1 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[2 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		mesh->m_Vertices[3 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
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
		mesh->m_Vertices[4 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f); //GREEN
		mesh->m_Vertices[5 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[6 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		mesh->m_Vertices[7 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
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
		mesh->m_Vertices[8 ].a_Colour = vec4(0.f, 0.f, 1.f, 1.f); //BLUE
		mesh->m_Vertices[9 ].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[10].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[11].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[8 ].a_UV = vec2(0.f, 0.f);
		mesh->m_Vertices[9 ].a_UV = vec2(0.f, 1.f);
		mesh->m_Vertices[10].a_UV = vec2(1.f, 1.f);
		mesh->m_Vertices[11].a_UV = vec2(1.f, 0.f);

		mesh->m_Vertices[12].a_Position = vec3(+widthover2, -depthover2, -heightover2); //RIGHT
		mesh->m_Vertices[13].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		mesh->m_Vertices[14].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		mesh->m_Vertices[15].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		mesh->m_Vertices[12].a_Normal = vec3(+1.f,  0.f,  0.f);
		mesh->m_Vertices[13].a_Normal = vec3(+1.f,  0.f,  0.f);
		mesh->m_Vertices[14].a_Normal = vec3(+1.f,  0.f,  0.f);
		mesh->m_Vertices[15].a_Normal = vec3(+1.f,  0.f,  0.f);
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
		mesh->m_Vertices[16].a_Normal = vec3( 0.f, +1.f,  0.f);
		mesh->m_Vertices[17].a_Normal = vec3( 0.f, +1.f,  0.f);
		mesh->m_Vertices[18].a_Normal = vec3( 0.f, +1.f,  0.f);
		mesh->m_Vertices[19].a_Normal = vec3( 0.f, +1.f,  0.f);
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
		mesh->m_Vertices[20].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[21].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[22].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[23].a_Normal = vec3( 0.f, -1.f,  0.f);
		mesh->m_Vertices[20].a_Colour = vec4(1.f, 0.f, 1.f, 1.f); //PURPLE
		mesh->m_Vertices[21].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[22].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		mesh->m_Vertices[23].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
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

	//Ref<Mesh> MeshFactory::generateCurve(curvefunc_t curvefunc, float tstart, float tend, const int tsteps, curvecolourfunc_t curvecolourfunc)
	//{
	//	//Ref<Mesh>& mesh = MakeRef<Mesh>(tsteps);
	//}

	Ref<Mesh> MeshFactory::generateSurface(surfacefunc_t curvefunc, float ustart, float uend, const int usteps, float vstart, float vend, const int vsteps, surfacecolourfunc_t curvecolourfunc)
	{
		Ref<Mesh> mesh = MakeRef<Mesh>(usteps * vsteps, (usteps - 1) * (vsteps - 1) * 6);

		for (int y = 0; y < vsteps; y++)
		{
			float v = (float)y / vsteps * (vend - vstart) + vstart;
			for (int x = 0; x < usteps; x++)
			{
				float u = (float)x / usteps * (uend - ustart) + ustart;

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