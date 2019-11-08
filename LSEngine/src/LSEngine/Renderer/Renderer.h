#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Core/Camera/Camera.h"
#include "LSEngine/Renderer/RenderCommand.h"
#include "LSEngine/Renderer/Shader.h"

namespace LSE {

	class LSE_API Renderer
	{
	public:
		static void BeginScene(const Ref<Camera3D>& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::vec3 ViewPosition;
			glm::vec3 ViewDir;
		};

		static Ref<SceneData> s_SceneData;
	};
}