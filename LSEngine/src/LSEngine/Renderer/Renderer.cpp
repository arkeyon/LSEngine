#include "lsepch.h"
#include "Renderer.h"

namespace LSE {

	Ref<Renderer::SceneData> Renderer::s_SceneData(new Renderer::SceneData());

	void Renderer::BeginScene(const Ref<Camera3D>& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetVP();
		s_SceneData->ViewPosition = camera->GetPos();
		s_SceneData->ViewDir = camera->GetDir();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		shader->SetUniformMat4("u_VP", s_SceneData->ViewProjectionMatrix);
		shader->SetUniformMat4("u_ModelMatrix", transform);
		shader->SetUniform3f("u_EyeDir", s_SceneData->ViewDir);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}