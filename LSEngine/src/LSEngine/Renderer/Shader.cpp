#include "lsepch.h"

#include "Shader.h"
#include <fstream>
#include <iostream>

#include "LSEngine/IOUtils.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace LSE {

	Shader* Shader::Create(std::string vertPath, std::string fragPath)
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			LSE_CORE_ASSERT(false, "Cant create Shader without RenderingAPI");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertPath, fragPath);
		}
		return nullptr;

	}

}