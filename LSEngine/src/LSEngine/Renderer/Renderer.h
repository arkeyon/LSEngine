#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Core/Camera/Camera.h"
#include "LSEngine/Renderer/RenderCommand.h"
#include "LSEngine/Renderer/Shader.h"
#include "LSEngine/Renderer/Model.h"

namespace LSE {

	class LSE_API Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Ref<Camera3D>& camera);
		static void EndScene();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));
		static void Submit(const Ref<Shader>& shader,  Ref<Model> model, const glm::mat4& transform = glm::mat4(1.f));

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