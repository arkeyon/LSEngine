#include "lsepch.h"

#include "OpenGlShader.h"
#include <fstream>
#include <iostream>

#include "LSEngine/Core/IOUtils.h"

namespace LSE {

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

	GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment") return GL_FRAGMENT_SHADER;
		if (type == "geometry") return GL_GEOMETRY_SHADER;
		return GL_NONE;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		m_ShaderProgram = glCreateProgram();

		std::vector<GLuint> shaderIDs;

		for (auto shader : shaderSources)
		{
			GLenum type = shader.first;
			const std::string& source = shader.second;

			GLuint shader = glCreateShader(type);

			const GLchar* csource = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &csource, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);
				LSE_CORE_ERROR("{0}", infoLog.data());
				LSE_CORE_ASSERT(false, "Shader Failed to Compile");
				return;
			}

			glAttachShader(m_ShaderProgram, shader);
			shaderIDs.push_back(shader);
		}
		glLinkProgram(m_ShaderProgram);

		GLint isLinked = 0;
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ShaderProgram, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(m_ShaderProgram);

			for (auto shader : shaderIDs)
			{
				glDeleteShader(shader);
			}

			LSE_CORE_ERROR("{0}", infoLog.data());
			LSE_CORE_ASSERT(false, "Shader Failed to Link");
			return;
		}

		for (auto shader : shaderIDs)
		{
			glDeleteShader(shader);
		}
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_ShaderProgram);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformMat4(const char* name, const glm::mat4& matrix)
	{
		GLuint location = glGetUniformLocation(m_ShaderProgram, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, &(matrix[0][0]));
	}

	void OpenGLShader::SetUniformi(const char* name, int i)
	{
		GLuint location = glGetUniformLocation(m_ShaderProgram, name);
		glUniform1i(location, i);
	}

	void OpenGLShader::SetUniform4f(const char* name, const glm::vec4& vector)
	{
		GLuint location = glGetUniformLocation(m_ShaderProgram, name);
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::SetUniform3f(const char* name, const glm::vec3& vector)
	{
		GLuint location = glGetUniformLocation(m_ShaderProgram, name);
		glUniform3f(location, vector.x, vector.y, vector.z);
	}
	
	void OpenGLShader::SetUniform2f(const char* name, const glm::vec2& vector)
	{
		GLuint location = glGetUniformLocation(m_ShaderProgram, name);
		glUniform2f(location, vector.x, vector.y);
	}
	
	void OpenGLShader::SetUniform1f(const char* name, const float& vector)
	{
		GLuint location = glGetUniformLocation(m_ShaderProgram, name);
		glUniform1f(location, vector);
	}
}