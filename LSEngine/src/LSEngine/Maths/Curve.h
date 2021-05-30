#pragma once

#include <glm/glm.hpp>
#include "LSEngine/ECS/Objects/ECS.h"

namespace LSE::Maths {

	class ParametricCurve;
	class Parabola;
	class Line;

	template <class firstclass, class secondclass>
	std::vector<glm::vec3> intercept(const Ref<firstclass> first, const Ref<secondclass> second)
	{
		std::vector<glm::vec3> solutions;

		auto sol = interceptLocal(first, second);
		for (auto s : sol)
		{
			solutions.push_back(glm::vec3(first->GetComponent<ReferenceFrame>()->getModelMat() * glm::vec4(first->function(s.x), 1.f) + second->GetComponent<ReferenceFrame>()->getModelMat() * glm::vec4(second->function(s.y), 1.f)) / 2.f);
		}

		return solutions;
	}

	std::vector<std::pair<float, float>> interceptLocal(const Ref<ParametricCurve> first, const Ref<ParametricCurve> second);

	class ParametricCurve : public LSE::Entity
	{
	public:
		ParametricCurve(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin = -30.f, const float& tmax = 30.f);

		virtual glm::vec3 function(const float& t) const = 0;
		virtual glm::vec3 gradiant(const float& t) const;

		float m_TMin;
		float m_TMax;
	};

	std::vector<glm::vec2> interceptLocal(const Ref<Parabola> first, const Ref<Parabola> second);
	std::vector<glm::vec2> interceptLocal(const Ref<Parabola> first, const Ref<Line> second);
	std::vector<glm::vec2> interceptLocal(const Ref<Line> first, const Ref<Parabola> second);

	class Parabola : public ParametricCurve
	{
	public:
		Parabola(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin = -30.f, const float& tmax = 30.f);

		virtual glm::vec3 function(const float& t) const override;
		virtual glm::vec3 gradiant(const float& t) const override;

	private:
		static Ref<Model> m_Model;
	};

	class Line : public ParametricCurve
	{
	public:
		Line(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin = -30.f, const float& tmax = 30.f);

		virtual glm::vec3 function(const float& t) const override;
		virtual glm::vec3 gradiant(const float& t) const override;

	private:
		static Ref<Model> m_Model;
	};

}