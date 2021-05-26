#include "lsepch.h"

#include "Curve.h"

namespace LSE::Maths {

	ParametricCurve::ParametricCurve(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale)
	{
		AddComponent<ReferenceFrame>(pos, orin, scale);
	}

	glm::vec3 ParametricCurve::gradiant(const float& t) const
	{
		float epsilon = 0.0001f;

		return (function(t + epsilon) - function(t)) / epsilon;
	}

	std::vector<std::pair<float, float>> ParametricCurve::interceptLocal(const ParametricCurve& other)
	{
		return std::vector<std::pair<float, float>>();
	}

	std::vector<glm::vec3> ParametricCurve::intercept(const ParametricCurve& other)
	{
		std::vector<glm::vec3> solutions;

		auto sol = interceptLocal(other);
		for (auto s : sol)
		{
			solutions.push_back((function(s.first) + other.function(s.second)) / 2.f);
		}

		return solutions;
	}

	Parabola::Parabola(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale)
		: ParametricCurve(pos, orin, scale)
	{

	}

	glm::vec3 Parabola::function(const float& t) const { return glm::vec3(t, t * t, 0.f); }
	glm::vec3 Parabola::gradiant(const float& t) const { return glm::vec3(1.f, 2.f * t, 0.f); }

	std::vector<std::pair<float, float>> Parabola::interceptLocal(const Parabola& other)
	{
		glm::mat4 A = GetComponent<ReferenceFrame>()->getModelMat();
		glm::mat4 B = other.GetComponent<ReferenceFrame>()->getModelMat();

		solveQuadQuad(A, B, )
	}

	std::vector<glm::vec3> Parabola::intercept(const Parabola& other)
	{
		std::vector<glm::vec3> solutions;

		auto sol = interceptLocal(other);
		for (auto s : sol)
		{
			solutions.push_back((function(s.first) + other.function(s.second)) / 2.f);
		}

		return solutions;
	}
	
	Line::Line(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale)
		: ParametricCurve(pos, orin, scale)
	{

	}

	glm::vec3 Line::function(const float& t) const { return glm::vec3(t, 0.f, 0.f); }
	glm::vec3 Line::gradiant(const float& t) const { return glm::vec3(1.f, 0.f, 0.f); }

	std::vector<glm::vec3> Line::intercept(const Parabola& other)
	{
		std::vector<glm::vec3> solutions;

		auto sol = interceptLocal(other);
		for (auto s : sol)
		{
			solutions.push_back((function(s.first) + other.function(s.second)) / 2.f);
		}

		return solutions;
	}

	std::vector<std::pair<float, float>> Line::interceptLocal(const Parabola& other)
	{
		std::vector<std::pair<float, float>> solutions;

		glm::mat4 A = other.GetComponent<ReferenceFrame>()->getModelMat();
		glm::mat4 B = GetComponent<ReferenceFrame>()->getModelMat();

		auto systemsolutions = solveQuadratic(A[1][0] * B[0][1] - A[1][1] * B[0][0], A[0][0] * B[0][1] - A[0][1] * B[0][0], B[0][1] * (A[3][0] - B[3][0]) + B[0][0] * (B[3][1] - A[3][1]));

		for (auto s : systemsolutions)
		{
			if (B[0][0] != 0.f) solutions.push_back(std::pair<float, float>((A[0][0] * s + A[1][0] * s * s + A[3][0] - B[3][0]) / B[0][0], s));
			else solutions.push_back(std::pair<float, float>((A[0][1] * s + A[1][1] * s * s + A[3][1] - B[3][1]) / B[0][1], s));
		}

		return solutions;
	}
	

}