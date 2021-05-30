#include "lsepch.h"

#include "LSEMath.h"

#include <array>
#include <vector>

namespace LSE::Maths {

	glm::vec2 solveQuadQuad(const glm::mat4& A, const glm::mat4& B, glm::vec2 start)
	{
		glm::vec2 t = start;

		float step = 0.001f;
		float stepcount = 2000.f;

		float epsilon = 0.06f;

		std::array<float, 5> pastchange;
		float lastl = 1000000.f;

		for (int i = 0; i < (int)stepcount; i++)
		{
			glm::vec4 T1(t[0], t[0] * t[0], 0.f, 1.f);
			glm::vec4 T2(t[1], t[1] * t[1], 0.f, 1.f);

			glm::vec4 C = A * T1 - B * T2;

			glm::mat4 R(1.f);
			R[0][0] = A[0][0] + 2.f * A[1][0] * t[0];
			R[1][0] = A[0][1] + 2.f * A[1][1] * t[0];
			R[0][1] = -B[0][0] + 2.f * -B[1][0] * t[1];
			R[1][1] = -B[0][1] + 2.f * -B[1][1] * t[1];

			float l = glm::length(C);
			float lchange = lastl - l;

			if (lchange < 0.f)
			{
				step *= 0.6f;
			}

			if (l < epsilon)
			{
				break;
			}

			glm::vec2 G = R * C / l;

			t -= G * (step + 0.08f * (1.f - i / (stepcount - 1.f)));
			lastl = l;
		}

		return t;
	}

	std::vector<float> solveQuadratic(float a, float b, float c)
	{
		std::vector<float> solutions;

		float det = b * b - 4.f * a * c;
		if (det < 0.f) return solutions;
		else if (det == 0.f)
		{
			solutions.push_back(-b / 2.f / a);
			return solutions;
		}

		float sq = sqrtf(det);

		float s1 = (-b + sq) / 2.f / a;
		float s2 = (-b - sq) / 2.f / a;

		solutions.push_back(s1);
		solutions.push_back(s2);

		return solutions;
	}

}