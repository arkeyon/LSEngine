#pragma once

#include "LSEngine/Core.h"
#include "RendererAPI.h"

namespace LSE {

	class LSE_API RenderCommand
	{
	public:
		inline static void SetClearColour(const glm::vec4& colour) { s_RenderAPI->SetClearColour(colour); }
		inline static void Clear() { s_RenderAPI->Clear(); }


		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_RenderAPI->DrawIndexed(vertexArray); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}