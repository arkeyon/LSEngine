#include "lsepch.h"
#include "Renderer.h"

namespace LSE {

	void Renderer::BeginScene(const Camera3D& camera)
	{
		
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}