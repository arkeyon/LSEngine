#pragma once

#include "LSEngine/Renderer/Shader.h"
#include <glad/glad.h>

namespace LSE {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& path);

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetUniformMat4(const char* name, const glm::mat4& matrix) override;
		virtual void SetUniformi(const char* name, int i) override;
		virtual void SetUniform4f(const char* name, const glm::vec4& vector) override;
		virtual void SetUniform3f(const char* name, const glm::vec3& vector) override;
		virtual void SetUniform2f(const char* name, const glm::vec2& vector) override;
		virtual void SetUniform1f(const char* name, const float& vector) override;

		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);
	private:
		uint32_t m_ShaderProgram;

		std::unordered_map<const char*, GLuint> m_NameMap;

		GLuint ShaderCache(const char* name);
	};

}