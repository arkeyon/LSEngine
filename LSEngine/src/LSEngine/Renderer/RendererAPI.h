#pragma once

#include "LSEngine/Core.h"
#include "VertexArray.h"
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

		virtual void Clear() = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void EnableDepthTest(bool enabled) = 0;
		virtual void EnableFaceCulling(bool enabled) = 0;
		virtual void EnableWireframe(bool enabled) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
		inline static void SetAPI(API api) { s_API = api; }
	private:
		static API s_API;
	};

}