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

	void MeshFactory::generateSphere(vertex_t* vertices, uint32_t* indices, const float& radius, const int& detail)
	{
		int width = detail * 2;
		int height = detail;

		for (int y = 0; y < height; y++)
		{
			float pitch = (y - (height - 1.f) / 2.f) / (height - 1.f) * glm::pi<float>();

			float rr = cos(pitch);

			for (int x = 0; x < width; x++)
			{
				float yaw = x / (width - 1.f) * 2.f * glm::pi<float>();

				vertices[x + y * width].a_Normal = glm::vec3(cos(yaw) * rr, sin(yaw) * rr, sin(pitch));
				vertices[x + y * width].a_Position = vertices[x + y * width].a_Normal * radius;
				//vertices[x + y * width].a_Colour = glm::mix(glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec4(0.f, 0.f, 1.f, 1.f), (float)y / height);
				vertices[x + y * width].a_Colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
			}
		}

		for (int y = 0; y < height - 1; y++)
		{
			for (int x = 0; x < width - 1; x++)
			{
				for (int n = 0; n < 6; n++)
				{
					glm::ivec2 v = tvertices[tindices[n]];
					indices[(x + y * (width - 1)) * 6 + n] = (v.x + x) + (v.y + y) * width;
				}
			}
		}
		//if (numofvertices)*numofvertices = width * height;
		//if (numofindices)*numofindices = (width - 1) * (height - 1) * 6;
	}

	void MeshFactory::generateRectCorner(vertex_t* vertices, uint32_t* indices, float width, float height, float depth)
	{
		using namespace glm;

		vertices[0].a_Position = vec3(0.f, 0.f, 0.f);
		vertices[1].a_Position = vec3(0.f, height, 0.f);
		vertices[2].a_Position = vec3(width, height, 0.f);
		vertices[3].a_Position = vec3(width, 0.f, 0.f);
		vertices[4].a_Position = vec3(0.f, 0.f, depth);
		vertices[5].a_Position = vec3(0.f, height, depth);
		vertices[6].a_Position = vec3(width, height, depth);
		vertices[7].a_Position = vec3(width, 0.f, depth);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 3;
		indices[3] = 3;
		indices[4] = 1;
		indices[5] = 2;

		indices[6] = 3;
		indices[7] = 2;
		indices[8] = 7;
		indices[9] = 7;
		indices[10] = 2;
		indices[11] = 6;

		indices[12] = 7;
		indices[13] = 6;
		indices[14] = 4;
		indices[15] = 4;
		indices[16] = 6;
		indices[17] = 5;

		indices[18] = 4;
		indices[19] = 5;
		indices[20] = 0;
		indices[21] = 0;
		indices[22] = 5;
		indices[23] = 1;

		indices[24] = 1;
		indices[25] = 5;
		indices[26] = 2;
		indices[27] = 2;
		indices[28] = 5;
		indices[29] = 6;

		indices[30] = 7;
		indices[31] = 4;
		indices[32] = 3;
		indices[33] = 3;
		indices[34] = 4;
		indices[35] = 0;
	}

	void MeshFactory::generateRectCenterSharedVertices(vertex_t* vertices, uint32_t* indices, float width, float height, float depth)
	{
		float depthover2 = depth / 2.f;
		float widthover2 = width / 2.f;
		float heightover2 = height / 2.f;

		using namespace glm;
		vertices[0].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		vertices[1].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		vertices[2].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		vertices[3].a_Position = vec3(+widthover2, -depthover2, -heightover2);
		vertices[4].a_Position = vec3(-widthover2, -depthover2, +heightover2);
		vertices[5].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		vertices[6].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		vertices[7].a_Position = vec3(+widthover2, -depthover2, +heightover2);

		vertices[0].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		vertices[1].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		vertices[2].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		vertices[3].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		vertices[4].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		vertices[5].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		vertices[6].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		vertices[7].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 3;
		indices[3] = 3;
		indices[4] = 1;
		indices[5] = 2;

		indices[6] = 3;
		indices[7] = 2;
		indices[8] = 7;
		indices[9] = 7;
		indices[10] = 2;
		indices[11] = 6;

		indices[12] = 7;
		indices[13] = 6;
		indices[14] = 4;
		indices[15] = 4;
		indices[16] = 6;
		indices[17] = 5;

		indices[18] = 4;
		indices[19] = 5;
		indices[20] = 0;
		indices[21] = 0;
		indices[22] = 5;
		indices[23] = 1;

		indices[24] = 1;
		indices[25] = 5;
		indices[26] = 2;
		indices[27] = 2;
		indices[28] = 5;
		indices[29] = 6;

		indices[30] = 7;
		indices[31] = 4;
		indices[32] = 3;
		indices[33] = 3;
		indices[34] = 4;
		indices[35] = 0;
	}

	void MeshFactory::generateRectCenter(vertex_t* vertices, uint32_t* indices, float width, float height, float depth)
	{
		float depthover2 = depth / 2.f;
		float widthover2 = width / 2.f;
		float heightover2 = height / 2.f;
	
		using namespace glm;
		vertices[0 ].a_Position = vec3(-widthover2, -depthover2, -heightover2); //BOTTOM
		vertices[1 ].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		vertices[2 ].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		vertices[3 ].a_Position = vec3(+widthover2, -depthover2, -heightover2);								
		vertices[0 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		vertices[1 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		vertices[2 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		vertices[3 ].a_Normal = vec3( 0.f,  0.f, -1.f);
		vertices[0 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f); //RED
		vertices[1 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		vertices[2 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
		vertices[3 ].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);
	
		vertices[4 ].a_Position = vec3(-widthover2, +depthover2, +heightover2); //TOP
		vertices[5 ].a_Position = vec3(-widthover2, -depthover2, +heightover2);
		vertices[6 ].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		vertices[7 ].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		vertices[4 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		vertices[5 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		vertices[6 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		vertices[7 ].a_Normal = vec3( 0.f,  0.f, +1.f);
		vertices[4 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f); //GREEN
		vertices[5 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		vertices[6 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
		vertices[7 ].a_Colour = vec4(0.f, 1.f, 0.f, 1.f);
	
		vertices[8 ].a_Position = vec3(-widthover2, -depthover2, +heightover2); //LEFT
		vertices[9 ].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		vertices[10].a_Position = vec3(-widthover2, +depthover2, -heightover2);
		vertices[11].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		vertices[8 ].a_Normal = vec3(-1.f,  0.f,  0.f);
		vertices[9 ].a_Normal = vec3(-1.f,  0.f,  0.f);
		vertices[10].a_Normal = vec3(-1.f,  0.f,  0.f);
		vertices[11].a_Normal = vec3(-1.f,  0.f,  0.f);
		vertices[8 ].a_Colour = vec4(0.f, 0.f, 1.f, 1.f); //BLUE
		vertices[9 ].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		vertices[10].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
		vertices[11].a_Colour = vec4(0.f, 0.f, 1.f, 1.f);
	
		vertices[12].a_Position = vec3(+widthover2, -depthover2, -heightover2); //RIGHT
		vertices[13].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		vertices[14].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		vertices[15].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		vertices[12].a_Normal = vec3(+1.f,  0.f,  0.f);
		vertices[13].a_Normal = vec3(+1.f,  0.f,  0.f);
		vertices[14].a_Normal = vec3(+1.f,  0.f,  0.f);
		vertices[15].a_Normal = vec3(+1.f,  0.f,  0.f);
		vertices[12].a_Colour = vec4(1.f, 1.f, 0.f, 1.f); //YELLOW
		vertices[13].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		vertices[14].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
		vertices[15].a_Colour = vec4(1.f, 1.f, 0.f, 1.f);
	
		vertices[16].a_Position = vec3(-widthover2, +depthover2, -heightover2); //BACK
		vertices[17].a_Position = vec3(-widthover2, +depthover2, +heightover2);
		vertices[18].a_Position = vec3(+widthover2, +depthover2, +heightover2);
		vertices[19].a_Position = vec3(+widthover2, +depthover2, -heightover2);
		vertices[16].a_Normal = vec3( 0.f, +1.f,  0.f);
		vertices[17].a_Normal = vec3( 0.f, +1.f,  0.f);
		vertices[18].a_Normal = vec3( 0.f, +1.f,  0.f);
		vertices[19].a_Normal = vec3( 0.f, +1.f,  0.f);
		vertices[16].a_Colour = vec4(0.f, 1.f, 1.f, 1.f); //AQUA
		vertices[17].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		vertices[18].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
		vertices[19].a_Colour = vec4(0.f, 1.f, 1.f, 1.f);
	
		vertices[20].a_Position = vec3(-widthover2, -depthover2, +heightover2); //FRONT
		vertices[21].a_Position = vec3(-widthover2, -depthover2, -heightover2);
		vertices[22].a_Position = vec3(+widthover2, -depthover2, -heightover2);
		vertices[23].a_Position = vec3(+widthover2, -depthover2, +heightover2);
		vertices[20].a_Normal = vec3( 0.f, -1.f,  0.f);
		vertices[21].a_Normal = vec3( 0.f, -1.f,  0.f);
		vertices[22].a_Normal = vec3( 0.f, -1.f,  0.f);
		vertices[23].a_Normal = vec3( 0.f, -1.f,  0.f);
		vertices[20].a_Colour = vec4(1.f, 0.f, 1.f, 1.f); //PURPLE
		vertices[21].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		vertices[22].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
		vertices[23].a_Colour = vec4(1.f, 0.f, 1.f, 1.f);
	
		indices[0 ] = 0; //BOTTOM
		indices[1 ] = 1;
		indices[2 ] = 3;
		indices[3 ] = 3;
		indices[4 ] = 1;
		indices[5 ] = 2;
	
		indices[6 ] = 4; //TOP
		indices[7 ] = 5;
		indices[8 ] = 7;
		indices[9 ] = 7;
		indices[10] = 5;
		indices[11] = 6;
	
		indices[12] = 8; //LEFT
		indices[13] = 9;
		indices[14] = 11;
		indices[15] = 11;
		indices[16] = 9;
		indices[17] = 10;
	
		indices[18] = 12; //RIGHT
		indices[19] = 13;
		indices[20] = 15;
		indices[21] = 15;
		indices[22] = 13;
		indices[23] = 14;
	
		indices[24] = 16; //BACK
		indices[25] = 17;
		indices[26] = 19;
		indices[27] = 19;
		indices[28] = 17;
		indices[29] = 18;
	
		indices[30] = 20; //FRONT
		indices[31] = 21;
		indices[32] = 23;
		indices[33] = 23;
		indices[34] = 21;
		indices[35] = 22;
	}
}