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
		virtual void SetUniform4fv(const char* name, const glm::vec4& vector) override;
	private:
		uint32_t m_ShaderProgram;
	};

}