#pragma once

#include "LSEngine/Core.h"

#include <glm/glm.hpp>
#include <string>

namespace LSE {

	class LSE_API Shader
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetUniformMat4(const char* name, const glm::mat4& matrix) = 0;
		virtual void SetUniformi(const char* name, int i) = 0;
		virtual void SetUniform4fv(const char* name, const glm::vec4& vector) = 0;
		static Shader* Create(std::string vertexPath, std::string fragmentPath);
	};

}