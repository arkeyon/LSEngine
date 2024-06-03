#include <LSEngine.h>

#include "imgui.h"
#include "imgui_internal.h"

#include "LSEngine/Core/Core.h"
#include "LSEngine/Core/IOUtils.h"
#include "LSEngine/Renderer/VertexArray.h"
#include "LSEngine/Renderer/Meshfactory.h"
#include "LSEngine/Renderer/Renderer.h"
#include "LSEngine/Core/Camera/PerspectiveCameraController.h"
#include "LSEngine/Core/Camera/OrthographicCameraController.h"
#include "LSEngine/Core/Camera/OrthographicCamera.h"
#include "LSEngine/Renderer/Shader.h"
#include "LSEngine/Renderer/Texture.h"
#include "LSEngine/Maths/LSEMath.h"

#include <gl/GL.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/quaternion.hpp>

#include <stdio.h>

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Shader> m_Shader;
	//LSE::Ref<LSE::OrthographicCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;

	LSE::Ref<LSE::Model> m_Hill1;
	LSE::Ref<LSE::Model> m_Hill2;
	LSE::Ref<LSE::Model> m_Cube;

	bool m_Paused = false;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	glm::vec3 p2 = glm::vec3(70.f, 0.f, 0.f);
	float steepness = 1.f / 20.f;

public:

	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		float AR = (float)LSE::Application::Get().GetWindow().GetWidth() / (float)LSE::Application::Get().GetWindow().GetHeight();

		//m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), -7.f, 7.f, -7.f / AR, 7.f / AR, -1.f, 1.f);
		m_Camera = MakeRef<PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		m_CameraController = MakeRef<PerspectiveCameraController>(m_Camera);

		m_Shader.reset(Shader::Create("assets/shaders/bridgeshader.glsl"));

		m_Hill1 = MakeRef<Model>();
		Ref<Mesh> hill = MeshFactory3D::paramatricSurface([](const float& u, const float& v)
			{

				return glm::vec3(u, v, -(u * u + v * v));

			}, -50.f, 50.f, 100, -50.f, 50.f, 100);
		//hill->Invert();
		m_Hill1->AddMesh(hill);

		m_Hill2 = MakeRef<Model>();
		m_Hill2->AddMesh(MeshFactory3D::paramatricSurface([](const float& u, const float& v)
			{

				return glm::vec3(u, v, -(u * u + v * v));

			}, -50.f, 50.f, 100, -50.f, 50.f, 100));

		m_Cube = MakeRef<Model>();
		m_Cube->AddMesh(MeshFactory3D::cubeCenter(10.f));
		RenderCommand::SetClearColour(glm::vec4(0.f, 0.1f, 0.f, 1.f));
		RenderCommand::EnableDepthTest(true);
		RenderCommand::EnableFaceCulling(false);
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			m_CameraController->OnUpdate(delta);
		}

		m_Frames += 1.f;
		m_Time += delta;

		if (m_Time > 1.f)
		{
			m_FPS = m_Frames / m_Time;
			m_Frames = 0.f;
			m_Time = 0.f;
		}
		glm::vec2 p22 = glm::vec2(p2);
		float l = glm::length(p22);
		p22 /= l;

		float a1 = -steepness, a2 = -steepness;
		float b2 = -2.f * l * a2;
		float c2 = l * l * a2 + p2.z;

		auto solutions = Maths::solveQuadratic(a2 - a2 * a2 / a1, -a2 / a1 * b2, -c2 - b2 * b2 / 4.f / a1);

		RenderCommand::Clear();
		Renderer::BeginScene(m_Camera);
		m_Shader->SetUniform3f("u_EyeDir", m_Camera->GetDir());

		if (solutions.size() != 0)
		{

			if (solutions.size() == 1)
			{
				float i2x = (2.f * a2 * solutions[0] + b2) / (2.f * a1);

				glm::vec3 i1 = glm::vec3(i2x * p22, a1 * i2x * i2x);
				glm::vec3 i2 = glm::vec3(solutions[0] * p22, a2 * solutions[0] * solutions[0] + b2 * solutions[0] + c2);
				//LSE_TRACE("{0}\t{1}\t{2}\t\t{3}\t{4}\t{5}", i1.x, i1.y, i1.z, i2.x, i2.y, i2.z);
				LSE_TRACE("{0}\t{1}", solutions[0], i2x);



				Ref<Model> L = MakeRef<Model>();
				L->AddMesh(MeshFactory2D::line(i1, i2));

				Renderer::Submit(m_Shader, L);
			}
			else
			{
				float i2x1 = (2.f * a2 * solutions[0] + b2) / (2.f * a1);
				float i2x2 = (2.f * a2 * solutions[1] + b2) / (2.f * a1);

				glm::vec3 i11 = glm::vec3(i2x1 * p22, a1 * i2x1 * i2x1);
				glm::vec3 i12 = glm::vec3(i2x2 * p22, a1 * i2x2 * i2x2);

				glm::vec3 i21 = glm::vec3(solutions[0] * p22, a2 * solutions[0] * solutions[0] + b2 * solutions[0] + c2);
				glm::vec3 i22 = glm::vec3(solutions[1] * p22, a2 * solutions[1] * solutions[1] + b2 * solutions[1] + c2);

				//LSE_TRACE("1\t{0}\t{1}\t{2}\t\t{3}\t{4}\t{5}", i11.x, i11.y, i11.z, i21.x, i21.y, i21.z);
				//LSE_TRACE("2\t{0}\t{1}\t{2}\t\t{3}\t{4}\t{5}", i12.x, i12.y, i12.z, i22.x, i22.y, i22.z);
				LSE_TRACE("{0}\t{1}\t{2}\t{3}", solutions[0], i2x1, solutions[1], i2x2);

				Ref<Model> L1 = MakeRef<Model>();
				L1->AddMesh(MeshFactory2D::line(i11, i21));
				Ref<Model> L2 = MakeRef<Model>();
				L2->AddMesh(MeshFactory2D::line(i12, i22));

				Renderer::Submit(m_Shader, L1);
				Renderer::Submit(m_Shader, L2);
			}
		}

		Renderer::Submit(m_Shader, m_Hill1, glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, steepness)));
		Renderer::Submit(m_Shader, m_Hill2, glm::translate(glm::mat4(1.f), p2) * glm::scale(glm::mat4(1.f), glm::vec3(1.f, 1.f, steepness)));
		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::SliderFloat3("Position2", &p2[0], -100.f, 100.f);
		ImGui::SliderFloat("Steepness", &steepness, 0.01f, 1.f);
		ImGui::End();
	}

	void OnEvent(LSE::Event& e) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			m_CameraController->OnEvent(e);
		}

		EventDispatcher kped(e);
		kped.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
			{
				if (e.GetKeyCode() == LSE_KEY_ESCAPE)
				{
					m_Paused = !m_Paused;
					auto& window = Application::Get().GetWindow();
					if (m_Paused) window.SetCursorState(true);
					else window.SetCursorState(false);
				}

				return false;
			});
	}
};

class Sandbox : public LSE::Application
{
public:
	Sandbox()
		: Application(1280, 720)
	{
		PushLayer(MakeScope<ExampleLayer>());
	}

	~Sandbox()
	{

	}
};

LSE::Application* LSE::CreateApplication()
{
	return new Sandbox();
}

