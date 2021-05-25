#pragma once

#include <array>

namespace LSE {

	std::array<float, 2> solveQuadratic(float a, float b, float c)
	{
		float det = b * b - 4.f * a * c;
		if (det < 0.f) return { NAN, NAN };

		float sq = sqrtf(det);

		float s1 = (-b + sq) / 2.f / a;
		float s2 = (-b - sq) / 2.f / a;

		return { s1, s2 };
	}

}