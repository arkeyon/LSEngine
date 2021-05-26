#pragma once

#include <glm/glm.hpp>
#include "LSEngine/ECS/Objects/ECS.h"
#include "LSEngine/Maths/LSEMath.h"

namespace LSE::Maths {

	class ParametricCurve : public LSE::Entity
	{
	public:
		ParametricCurve(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale);

		virtual glm::vec3 function(const float& t) const = 0;
		virtual glm::vec3 gradiant(const float& t) const;

		std::vector<std::pair<float, float>> interceptLocal(const ParametricCurve& other);

		std::vector<glm::vec3> intercept(const ParametricCurve& other);

	};

	class Parabola : public ParametricCurve
	{
	public:
		Parabola(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale);

		virtual glm::vec3 function(const float& t) const override;
		virtual glm::vec3 gradiant(const float& t) const override;

		std::vector<std::pair<float, float>> interceptLocal(const Parabola& other);

		std::vector<glm::vec3> intercept(const Parabola& other);
	};

	class Line : public ParametricCurve
	{
	public:
		Line(const glm::vec3& pos, const glm::quat& orin, const glm::vec3& scale);

		virtual glm::vec3 function(const float& t) const override;
		virtual glm::vec3 gradiant(const float& t) const override;

		std::vector<glm::vec3> intercept(const Parabola& other);

		std::vector<std::pair<float, float>> interceptLocal(const Parabola& other);
	};

}