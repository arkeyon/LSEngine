#include "lsepch.h"
#include "VertexArray.h"

#include "LSEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace LSE {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			LSE_CORE_ASSERT(false, "Cant create VertexBuffer without RenderingAPI");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return MakeRef<OpenGLVertexArray>();
		}
		return nullptr;
	}

}