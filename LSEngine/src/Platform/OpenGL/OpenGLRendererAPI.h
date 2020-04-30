#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/RendererAPI.h"

namespace LSE {

	class LSE_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Clear(bool Colour, bool Depth, bool Stencil) override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void EnableDepthTest(bool enabled) override;
		virtual void EnableFaceCulling(bool enabled) override;
		virtual void EnableWireframe(bool enabled) override;
		virtual void StencilDraw(bool enabled) override;
		virtual void EnableStencil(bool enabled) override;

		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}