#pragma once

#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/constants.hpp>

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

	void generateSphere(const float& radius, const int& detail, glm::vec3* vertices, int32_t* indices, int* numofvertices, int* numofindices, const int& indiceoffs, glm::bvec4 color)
	{
		int width = detail * 2;
		int height = detail;

		for (int y = 0; y < height; y++)
		{
			float pitch = (y - (height - 1.f) / 2.f) / (height - 1.f) * glm::pi<float>();

			float r = cos(pitch) * radius;

			for (int x = 0; x < width; x++)
			{
				float yaw = x / (width - 1.f) * 2.f * glm::pi<float>();

				vertices[x + y * width] = glm::vec3(cos(yaw) * r, sin(yaw) * r, sin(pitch) * radius);
			}
		}

		for (int y = 0; y < height - 1; y++)
		{
			for (int x = 0; x < width - 1; x++)
			{
				for (int n = 0; n < 6; n++)
				{
					glm::ivec2 v = tvertices[tindices[n]];
					indices[(x + y * (width - 1)) * 6 + n] = (v.x + x) + (v.y + y) * width + indiceoffs;
				}
			}
		}
		if (numofvertices)* numofvertices = width * height;
		if (numofindices)* numofindices = (width - 1) * (height - 1) * 6;
	}

	void generateRectCorner(float* vertices, int32_t* indices, float width, float height, float depth, int32_t stride = 4, int32_t offset = 0)
	{
		using namespace glm;

		*(glm::vec3*)((DWORD)vertices + 0 * stride + offset) = vec3(0.f, 0.f, 0.f);
		*(glm::vec3*)((DWORD)vertices + 1 * stride + offset) = vec3(0.f, height, 0.f);
		*(glm::vec3*)((DWORD)vertices + 2 * stride + offset) = vec3(width, height, 0.f);
		*(glm::vec3*)((DWORD)vertices + 3 * stride + offset) = vec3(width, 0.f, 0.f);
		*(glm::vec3*)((DWORD)vertices + 4 * stride + offset) = vec3(0.f, 0.f, depth);
		*(glm::vec3*)((DWORD)vertices + 5 * stride + offset) = vec3(0.f, height, depth);
		*(glm::vec3*)((DWORD)vertices + 6 * stride + offset) = vec3(width, height, depth);
		*(glm::vec3*)((DWORD)vertices + 7 * stride + offset) = vec3(width, 0.f, depth);

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

	void generateRectCenter(float* vertices, int32_t* indices, int32_t stride, int32_t offset, float width, float height, float depth)
	{
		float depthover2 = depth / 2.f;
		float widthover2 = width / 2.f;
		float heightover2 = height / 2.f;

		using namespace glm;
		*(glm::vec3*)((DWORD)vertices + 0 * stride + offset) = vec3(-widthover2, -heightover2, -depthover2);
		*(glm::vec3*)((DWORD)vertices + 1 * stride + offset) = vec3(-widthover2, +heightover2, -depthover2);
		*(glm::vec3*)((DWORD)vertices + 2 * stride + offset) = vec3(+widthover2, +heightover2, -depthover2);
		*(glm::vec3*)((DWORD)vertices + 3 * stride + offset) = vec3(+widthover2, -heightover2, -depthover2);
		*(glm::vec3*)((DWORD)vertices + 4 * stride + offset) = vec3(-widthover2, -heightover2, +depthover2);
		*(glm::vec3*)((DWORD)vertices + 5 * stride + offset) = vec3(-widthover2, +heightover2, +depthover2);
		*(glm::vec3*)((DWORD)vertices + 6 * stride + offset) = vec3(+widthover2, +heightover2, +depthover2);
		*(glm::vec3*)((DWORD)vertices + 7 * stride + offset) = vec3(+widthover2, -heightover2, +depthover2);

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
}