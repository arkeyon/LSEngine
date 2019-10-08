#pragma once

#include "LSEngine/Core.h"
#include "LSEngine/Renderer/RendererAPI.h"

namespace LSE {

	class LSE_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Clear() override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void EnableDepthTest() override;
		virtual void EnableFaceCulling() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}