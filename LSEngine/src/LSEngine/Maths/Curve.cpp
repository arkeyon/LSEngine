#include "lsepch.h"

#include "Curve.h"
#include "LSEngine/Renderer/Meshfactory.h"

namespace LSE::Maths {

	std::vector<glm::vec3> intercept(const Ref<ParametricCurve> first, const Ref<ParametricCurve> second)
	{
		std::vector<glm::vec3> solutions;

		auto sol = first->interceptLocal(second);
		for (auto s : sol)
		{
			solutions.push_back(glm::vec3(first->GetComponent<ReferenceFrame>()->getModelMat() * glm::vec4(first->function(s.x), 1.f) + second->GetComponent<ReferenceFrame>()->getModelMat() * glm::vec4(second->function(s.y), 1.f)) / 2.f);
		}

		return solutions;
	}

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

	std::vector<glm::vec2> ParametricCurve::interceptLocal(const Ref<ParametricCurve> second)
	{
		std::vector<glm::vec2> solutions;

		return solutions;
	}

	PointCurve::PointCurve(std::vector<glm::vec3>& points)
		: m_Points(points), ParametricCurve(glm::vec3(), glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f)), glm::vec3(1.f, 1.f, 1.f), m_Points[0].z, m_Points[m_Points.size() - 1].z)
	{

	}

	glm::vec3 PointCurve::function(const float& t) const
	{
		for (int i = 0; i < m_Points.size(); i++)
		{
			auto& m1 = m_Points[i];
			auto& m2 = m_Points[(i + 1 == m_Points.size()) ? 0 : i + 1];

			if (t > m1.z)
			{
				if (t <= m2.z)
				{
					return glm::mix(glm::vec3(m1.x, m1.y, 0.f), glm::vec3(m2.x, m2.y, 0.f), (t - m1.z) / (m2.z - m1.z));
				}
				continue;
			}
			else
			{
				continue;
			}
		}
	}

	UnnamedCurve::UnnamedCurve(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, func_t function, const float& tmin, const float& tmax, func_t gfunction)
		: ParametricCurve(pos, orin, scale, tmin, tmax), m_Func(function), m_GFunc(gfunction)
	{
		m_Model->AddMesh(MeshFactory2D::paramatric(function, tmin, tmax, 200));
	}

	glm::vec3 UnnamedCurve::function(const float& t) const { return m_Func(t); }

	glm::vec3 UnnamedCurve::gradiant(const float& t) const
	{
		if (m_GFunc) return m_GFunc(t);
		return ParametricCurve::gradiant(t);
	}

	Parabola::Parabola(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin, const float& tmax)
		: ParametricCurve(pos, orin, scale, tmin, tmax)
	{
		m_Model->AddMesh(MeshFactory2D::paramatric([](const float& t)
		{
			return glm::vec3(t, t * t, 0.f);
		}, tmin, tmax, 200));
	}

	glm::vec3 Parabola::function(const float& t) const { return glm::vec3(t, t * t, 0.f); }
	glm::vec3 Parabola::gradiant(const float& t) const { return glm::vec3(1.f, 2.f * t, 0.f); }

	std::vector<glm::vec2> Parabola::interceptLocal(const Ref<ParametricCurve> other)
	{
		if (std::dynamic_pointer_cast<Parabola>(other))
		{
			std::vector<glm::vec2> solutions;

			glm::mat4 A = GetComponent<ReferenceFrame>()->getModelMat();
			glm::mat4 B = other->GetComponent<ReferenceFrame>()->getModelMat();

			auto s1 = glm::vec2(m_TMin, other->m_TMin);
			auto s2 = glm::vec2(m_TMin, other->m_TMax);
			auto s3 = glm::vec2(m_TMax, other->m_TMin);
			auto s4 = glm::vec2(m_TMax, other->m_TMax);

			if (solveQuadQuad(A, B, s1)) if (s1.x > m_TMin && s1.x < m_TMax && s1.y > other->m_TMin && s1.y < other->m_TMax) solutions.push_back(s1);
			if (solveQuadQuad(A, B, s2)) if (s2.x > m_TMin && s2.x < m_TMax && s2.y > other->m_TMin && s2.y < other->m_TMax) solutions.push_back(s2);
			if (solveQuadQuad(A, B, s3)) if (s3.x > m_TMin && s3.x < m_TMax && s3.y > other->m_TMin && s3.y < other->m_TMax) solutions.push_back(s3);
			if (solveQuadQuad(A, B, s4)) if (s4.x > m_TMin && s4.x < m_TMax && s4.y > other->m_TMin && s4.y < other->m_TMax) solutions.push_back(s4);

			if (solutions.size() <= 1) return solutions;

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
		else if (std::dynamic_pointer_cast<Line>(other))
		{
			std::vector<glm::vec2> solutions;

			glm::mat4 A = GetComponent<ReferenceFrame>()->getModelMat();
			glm::mat4 B = other->GetComponent<ReferenceFrame>()->getModelMat();
			glm::mat4 C = glm::inverse(B) * A;

			auto systemsolutions = solveQuadratic(C[1][1], C[0][1], C[3][1]);

			for (auto s : systemsolutions)
			{
				auto s1 = glm::vec2(s, C[0][0] * s + C[1][0] * s * s + C[3][0]);
				if (s1.x > m_TMin && s1.x < m_TMax && s1.y > other->m_TMin && s1.y < other->m_TMax) solutions.push_back(s1);
			}

			return solutions;
		}

		return ParametricCurve::interceptLocal(other);
	}
	
	Line::Line(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin, const float& tmax)
		: ParametricCurve(pos, orin, scale, tmin, tmax)
	{
		m_Model->AddMesh(MeshFactory2D::line(glm::vec3(tmin, 0.f, 0.f), glm::vec3(tmax, 0.f, 0.f)));
	}

	glm::vec3 Line::function(const float& t) const { return glm::vec3(t, 0.f, 0.f); }
	glm::vec3 Line::gradiant(const float& t) const { return glm::vec3(1.f, 0.f, 0.f); }

	std::vector<glm::vec2> Line::interceptLocal(const Ref<ParametricCurve> other)
	{
		if (std::dynamic_pointer_cast<Parabola>(other))
		{
			std::vector<glm::vec2> solutions;

			glm::mat4 B = GetComponent<ReferenceFrame>()->getModelMat();
			glm::mat4 A = other->GetComponent<ReferenceFrame>()->getModelMat();
			glm::mat4 C = glm::inverse(B) * A;


			auto systemsolutions = solveQuadratic(C[1][1], C[0][1], C[3][1]);

			for (auto s : systemsolutions)
			{
				auto s1 = glm::vec2(C[0][0] * s + C[1][0] * s * s + C[3][0], s);
				if (s1.x > m_TMin && s1.x < m_TMax && s1.y > other->m_TMin && s1.y < other->m_TMax) solutions.push_back(s1);
			}

			return solutions;
		}
		else if (std::dynamic_pointer_cast<Line>(other))
		{
			std::vector<glm::vec2> solutions;

			glm::mat4 A = GetComponent<ReferenceFrame>()->getModelMat();
			glm::mat4 B = other->GetComponent<ReferenceFrame>()->getModelMat();
			glm::mat4 C = glm::inverse(B) * A;

			float s = -C[3][1] / C[0][1];
			auto s1 = glm::vec2(s, C[0][0] * s + C[3][0]);
			if (s1.x > m_TMin && s1.x < m_TMax && s1.y > other->m_TMin && s1.y < other->m_TMax) solutions.push_back(s1);

			return solutions;
		}

		return ParametricCurve::interceptLocal(other);
	}

	Ray::Ray(const glm::vec3 pos, const glm::quat& orin, const float& length)
		: ParametricCurve(pos, orin, glm::vec3(1.f, 1.f, 1.f), 0.f, length)
	{
		m_Model->AddMesh(MeshFactory2D::line(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f)));
	}

	glm::vec3 Ray::function(const float& t) const
	{
		if (t <= m_TMax) return glm::vec3(t, 0.f, 0.f);
		return glm::vec3();
	}

	glm::vec3 Ray::gradiant(const float& t) const
	{
		return glm::vec3();
	}

	std::vector<glm::vec2> Ray::interceptLocal(const Ref<ParametricCurve> other)
	{
		return { glm::vec3() };
	}

	void Ray::reflect(const Ref<ParametricCurve> other)
	{

	}
}