#pragma once

#include "LSEngine/Renderer/Shader.h"

namespace LSE {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string vertPath, std::string fragPath);

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetUniformMat4(const char* name, const glm::mat4& matrix) override;
		virtual void SetUniformi(const char* name, int i) override;
		virtual void SetUniform4f(const char* name, const glm::vec4& vector) override;
		virtual void SetUniform3f(const char* name, const glm::vec3& vector) override;
		virtual void SetUniform2f(const char* name, const glm::vec2& vector) override;
		virtual void SetUniform1f(const char* name, const float& vector) override;
	private:
		uint32_t m_ShaderProgram;
	};

}