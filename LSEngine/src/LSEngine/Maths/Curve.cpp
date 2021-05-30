#include "lsepch.h"

#include "Curve.h"
#include "LSEngine/Renderer/Meshfactory.h"
#include "LSEngine/Maths/LSEMath.h"

namespace LSE::Maths {

	ParametricCurve::ParametricCurve(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin, const float& tmax)
		: m_TMin(tmin), m_TMax(tmax)
	{
		AddComponent<ReferenceFrame>(pos, orin, scale);
	}

	glm::vec3 ParametricCurve::gradiant(const float& t) const
	{
		float epsilon = 0.0001f;

		return (function(t + epsilon) - function(t)) / epsilon;
	}

	Ref<Model> Parabola::m_Model = MakeRef<Model>();

	Parabola::Parabola(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin, const float& tmax)
		: ParametricCurve(pos, orin, scale, tmin, tmax)
	{
		if (!m_Model->m_Meshs.size()) m_Model->AddMesh(MeshFactory::paramatric([](const float& t)
			{
				return glm::vec3(t, t * t, 0.f);
			}, -10.f, 10.f, 200));

		AddComponent<Renderable>(m_Model);
	}

	glm::vec3 Parabola::function(const float& t) const { return glm::vec3(t, t * t, 0.f); }
	glm::vec3 Parabola::gradiant(const float& t) const { return glm::vec3(1.f, 2.f * t, 0.f); }

	std::vector<glm::vec2> interceptLocal(const Ref<Parabola> first, const Ref<Line> second)
	{
		std::vector<glm::vec2> solutions;

		glm::mat4 A = first->GetComponent<ReferenceFrame>()->getModelMat();
		glm::mat4 B = second->GetComponent<ReferenceFrame>()->getModelMat();
		glm::mat4 C = glm::inverse(B) * A;


		auto systemsolutions = solveQuadratic(C[1][1], C[0][1], C[3][1]);

		for (auto s : systemsolutions)
		{
			solutions.push_back(glm::vec2(s, C[0][0] * s + C[1][0] * s * s + C[3][0]));
		}

		return solutions;
	}

	std::vector<glm::vec2> interceptLocal(const Ref<Line> first, const Ref<Parabola> second)
	{
		std::vector<glm::vec2> solutions;

		glm::mat4 A = second->GetComponent<ReferenceFrame>()->getModelMat();
		glm::mat4 B = first->GetComponent<ReferenceFrame>()->getModelMat();
		glm::mat4 C = glm::inverse(B) * A;


		auto systemsolutions = solveQuadratic(C[1][1], C[0][1], C[3][1]);

		for (auto s : systemsolutions)
		{
			solutions.push_back(glm::vec2(C[0][0] * s + C[1][0] * s * s + C[3][0], s));
		}

		return solutions;
	}

	std::vector<glm::vec2> interceptLocal(const Ref<Parabola> first, const Ref<Parabola> second)
	{
		std::vector<glm::vec2> solutions;

		glm::mat4 A = first->GetComponent<ReferenceFrame>()->getModelMat();
		glm::mat4 B = second->GetComponent<ReferenceFrame>()->getModelMat();

		solutions.push_back(solveQuadQuad(A, B, glm::vec2(first->m_TMin, second->m_TMin)));
		solutions.push_back(solveQuadQuad(A, B, glm::vec2(first->m_TMin, second->m_TMax)));
		solutions.push_back(solveQuadQuad(A, B, glm::vec2(first->m_TMax, second->m_TMin)));
		solutions.push_back(solveQuadQuad(A, B, glm::vec2(first->m_TMax, second->m_TMax)));

		float epsilon = 0.01f;

		for (int i = 0; i < solutions.size() - 1; i++)
		{
			for (int j = 0; j < solutions.size(); j++)
			{
				if (i == j) continue;
				if (glm::length(solutions[i] - solutions[j]) < epsilon)
				{
					solutions.erase(solutions.begin() + j);
					j--;
				}
			}
		}

		return solutions;
	}

	Ref<Model> Line::m_Model = MakeRef<Model>();
	
	Line::Line(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin, const float& tmax)
		: ParametricCurve(pos, orin, scale, tmin, tmax)
	{
		if (!m_Model->m_Meshs.size()) m_Model->AddMesh(MeshFactory::line(glm::vec3(-30.f, 0.f, 0.f), glm::vec3(30.f, 0.f, 0.f)));

		AddComponent<Renderable>(m_Model);
	}

	glm::vec3 Line::function(const float& t) const { return glm::vec3(t, 0.f, 0.f); }
	glm::vec3 Line::gradiant(const float& t) const { return glm::vec3(1.f, 0.f, 0.f); }
	

}