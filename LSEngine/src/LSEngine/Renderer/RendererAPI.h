#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace LSE {

	class LSE_API RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:

		virtual void Clear(bool Colour, bool Depth, bool Stencil) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void EnableDepthTest(bool enabled) = 0;
		virtual void EnableFaceCulling(bool enabled) = 0;
		virtual void EnableWireframe(bool enabled) = 0;
		virtual void StencilDraw(int enabled) = 0;
		virtual void EnableStencil(bool enabled) = 0;
		virtual float GetPixelDepth(int x, int y) = 0;

		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
		inline static void SetAPI(API api) { s_API = api; }
	private:
		static API s_API;
	};

}