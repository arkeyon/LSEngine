#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/RendererAPI.h"

namespace LSE {

	class LSE_API RenderCommand
	{
	public:
		inline static void SetClearColour(const glm::vec4& colour) { s_RenderAPI->SetClearColour(colour); }
		inline static void Clear(bool Colour = true, bool Depth = true, bool Stencil = true) { s_RenderAPI->Clear(Colour, Depth, Stencil); }

		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RenderAPI->SetViewPort(x, y, width, height); }
		inline static void EnableDepthTest(bool enabled = true) { s_RenderAPI->EnableDepthTest(enabled); }
		inline static void EnableFaceCulling(bool enabled = true) { s_RenderAPI->EnableFaceCulling(enabled); }
		inline static void EnabledWireframe(bool enabled = true) { s_RenderAPI->EnableWireframe(enabled); };
		inline static void StencilDraw(bool enabled = true) { s_RenderAPI->StencilDraw(enabled); };
		inline static void EnableStencil(bool enabled = true) { s_RenderAPI->EnableStencil(enabled); };
		inline static float GetPixelDepth(int x, int y) { return s_RenderAPI->GetPixelDepth(x, y); };

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RenderAPI->DrawIndexed(vertexArray); }
	private:
		static RendererAPI* s_RenderAPI;
	};

}