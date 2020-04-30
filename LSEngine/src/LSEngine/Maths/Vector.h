#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace LSE { namespace Maths {

	static void AngleVectors(const glm::vec3& angles, glm::vec3* forward = nullptr, glm::vec3* side = nullptr, glm::vec3* up = nullptr)
	{
		float cospitch = cosf(angles.x);
		float sinpitch = sinf(angles.x);
		float cosyaw = cosf(angles.y);
		float sinyaw = sinf(angles.y);

		if (forward)* forward = glm::vec3(cosyaw * cospitch, sinyaw * cospitch, sinpitch);
		if (side)* side = glm::vec3(sinyaw, -cosyaw, 0.f);
		if (up)* up = glm::vec3(cosyaw * -sinpitch, sinyaw * -sinpitch, cospitch);
	}

	static void NormalizeAngles(glm::vec3& angles)
	{

		while (angles.x > glm::pi<float>()) angles.x -= glm::two_pi<float>(); //pitch
		while (angles.x <= -glm::pi<float>()) angles.x += glm::two_pi<float>();

		while (angles.y > glm::pi<float>()) angles.y -= glm::two_pi<float>();
		while (angles.y <= -glm::pi<float>()) angles.y += glm::two_pi<float>(); //yaw

		while (angles.z > glm::pi<float>()) angles.z -= glm::two_pi<float>();
		while (angles.z <= -glm::pi<float>()) angles.z += glm::two_pi<float>(); //roll
	}

	static glm::mat4 FPViewMatrix(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& side, const glm::vec3& up)
	{
		glm::mat4 Result(1.f);
		Result[0][0] = side.x;
		Result[0][1] = up.x;
		Result[0][2] = -forward.x;
		Result[1][0] = side.y;
		Result[1][1] = up.y;
		Result[1][2] = -forward.y;
		Result[2][0] = side.z;
		Result[2][1] = up.z;
		Result[2][2] = -forward.z;
		Result[3][0] = -glm::dot(side, pos);
		Result[3][1] = -glm::dot(up, pos);
		Result[3][2] = glm::dot(forward, pos);

		return Result;
	}

	static glm::mat4 FPViewMatrix(const glm::vec3& pos, const glm::vec3& angles, glm::vec3* forward = nullptr, glm::vec3* side = nullptr, glm::vec3* up = nullptr)
	{
		glm::vec3 f, s, u;

		AngleVectors(angles, &f, &s, &u);

		if (forward)* forward = f;
		if (side)* side = s;
		if (up)* up = u;

		glm::mat4 Result(1.f);
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[3][0] = -glm::dot(s, pos);
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[3][1] = -glm::dot(u, pos);
		Result[0][2] = -f.x;
		Result[1][2] = -f.y;
		Result[2][2] = -f.z;
		Result[3][2] = glm::dot(f, pos);

		return Result;
	}
}}