#pragma once

#include "LSEngine/Core.h"
#include "RenderCommand.h"

namespace LSE {

	class LSE_API Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}