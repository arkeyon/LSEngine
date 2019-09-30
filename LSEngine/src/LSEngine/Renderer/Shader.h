#pragma once

#include "LSEngine/Core.h"

#include <string>
#include <glad/glad.h>

namespace LSE {

	class LSE_API Shader
	{
	public:
		Shader(std::string vertexPath, std::string fragmentPath);
		void Bind();
		void Unbind();
	private:
		GLuint m_ShaderProgram;
	};

}