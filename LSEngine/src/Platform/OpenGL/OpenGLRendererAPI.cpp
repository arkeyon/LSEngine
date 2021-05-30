#include "lsepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace LSE {

	void OpenGLRendererAPI::Clear(bool Colour, bool Depth, bool Stencil)
	{
		glClear((Colour ? GL_COLOR_BUFFER_BIT : 0) | (Depth ? GL_DEPTH_BUFFER_BIT : 0) | (Stencil ? GL_STENCIL_BUFFER_BIT : 0));
	}

	void OpenGLRendererAPI::SetClearColour(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, RendererPrimitives primitives)
	{
		glDrawElements((GLuint)primitives, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::EnableDepthTest(bool enabled)
	{
		if (enabled) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::EnableFaceCulling(bool enabled)
	{
		if (enabled) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::EnableWireframe(bool enabled)
	{
		if (enabled) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void OpenGLRendererAPI::StencilDraw(int enabled)
	{
		if (enabled == 0)
		{
			glEnable(GL_STENCIL_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			//glStencilMask(0xFF);
			glStencilFunc(GL_NEVER, 0, 0xFF);
			glStencilOp(GL_INVERT, GL_KEEP, GL_KEEP);
		}
		else if (enabled == 1)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);
			//glStencilMask(0x00);
			glStencilFunc(GL_LEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}
		else if (enabled == 2)
		{
			glDisable(GL_STENCIL_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_TRUE);
			//glStencilMask(0x00);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}
	}

	void OpenGLRendererAPI::EnableStencil(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	float OpenGLRendererAPI::GetPixelDepth(int x, int y)
	{
		float depth;
		glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		return depth;
	}
}