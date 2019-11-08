#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/RendererAPI.h"

namespace LSE {

	class LSE_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Clear() override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void EnableDepthTest(bool enabled) override;
		virtual void EnableFaceCulling(bool enabled) override;
		virtual void EnableWireframe(bool enabled) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}