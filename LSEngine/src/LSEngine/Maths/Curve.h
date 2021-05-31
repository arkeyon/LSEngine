#pragma once

#include <glm/glm.hpp>
#include "LSEngine/ECS/Objects/ECS.h"
#include "LSEngine/Maths/LSEMath.h"

namespace LSE::Maths {

	class ParametricCurve;
	class Parabola;
	class Line;

	std::vector<glm::vec3> intercept(const Ref<ParametricCurve> first, const Ref<ParametricCurve> second);

	class ParametricCurve : public LSE::Entity
	{
	public:
		ParametricCurve(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin = -30.f, const float& tmax = 30.f);

		virtual glm::vec3 function(const float& t) const = 0;
		virtual glm::vec3 gradiant(const float& t) const;

		virtual std::vector<glm::vec2> interceptLocal(const Ref<ParametricCurve> other);

		float m_TMin;
		float m_TMax;
	};

	class Parabola : public ParametricCurve
	{
	public:
		Parabola(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin = -30.f, const float& tmax = 30.f);

		virtual glm::vec3 function(const float& t) const override;
		virtual glm::vec3 gradiant(const float& t) const override;

		virtual std::vector<glm::vec2> interceptLocal(const Ref<ParametricCurve> other) override;
	private:
		Ref<Model> m_Model;
	};

	class Line : public ParametricCurve
	{
	public:
		Line(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale, const float& tmin = -30.f, const float& tmax = 30.f);

		virtual glm::vec3 function(const float& t) const override;
		virtual glm::vec3 gradiant(const float& t) const override;

		virtual std::vector<glm::vec2> interceptLocal(const Ref<ParametricCurve> other) override;

	private:
		Ref<Model> m_Model;
	};
}