#include "lsepch.h"
#include "Renderer.h"

namespace LSE {

	std::shared_ptr<Renderer::SceneData> Renderer::s_SceneData(new Renderer::SceneData());

	void Renderer::BeginScene(const std::shared_ptr<Camera3D>& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetVP();
		s_SceneData->ViewPosition = camera->GetPos();
		s_SceneData->ViewDir = camera->GetDir();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		shader->SetUniformMat4("u_VP", s_SceneData->ViewProjectionMatrix);
		shader->SetUniformMat4("u_ModelMatrix", transform);
		shader->SetUniform3f("u_EyeDir", s_SceneData->ViewDir);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}