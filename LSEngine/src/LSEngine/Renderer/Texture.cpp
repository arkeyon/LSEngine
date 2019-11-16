#include "lsepch.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer.h"

namespace LSE {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			LSE_CORE_ASSERT(false, "Cant create Texture2D without RenderingAPI");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return MakeRef<OpenGLTexture2D>(path);
		}
		return nullptr;
	}

}