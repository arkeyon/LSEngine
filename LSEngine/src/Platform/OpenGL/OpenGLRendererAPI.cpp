#include "lsepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace LSE {

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColour(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
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
}