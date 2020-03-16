#include "lsepch.h"
#include "Shader.h"

#include "LSEngine/Core/IOUtils.h"
#include "LSEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace LSE {

	Shader* Shader::Create(const std::string& path)
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			LSE_CORE_ASSERT(false, "Cant create Shader without RenderingAPI");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(path);
		}
		return nullptr;

	}

}