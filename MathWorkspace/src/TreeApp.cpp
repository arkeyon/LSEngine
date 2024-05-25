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

#include <gl/GL.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>

#include <stdio.h>

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Shader> m_SimpleShader;
	LSE::Ref<LSE::OrthographicCamera> m_Camera;
	//LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	//LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;
	LSE::Ref<LSE::Model> m_SquareModel;
	LSE::Ref<LSE::Model> m_TriangleModel;

	bool m_Paused = false;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	float angle = glm::pi<float>() * 0.2152;
	float prevangle = glm::pi<float>() * 0.2152;
	int its = 0;
	float minsize = 0.1f;
	float prevminsize = 0.1f;
	float epsilon = 0.00000001f;

	bool sizechange = false;
	bool anglechange = false;

public:

	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		float AR = (float)LSE::Application::Get().GetWindow().GetWidth() / (float)LSE::Application::Get().GetWindow().GetHeight();

		m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), -7.f, 7.f, -7.f / AR, 7.f / AR, -1.f, 1.f);
		//m_Camera = MakeRef<PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
		//m_CameraController = MakeRef<PerspectiveCameraController>(m_Camera);

		m_SimpleShader.reset(Shader::Create("assets/shaders/treeshader.glsl"));

		m_SquareModel = MakeRef<LSE::Model>();
		Ref<Mesh> squaremesh = MeshFactory2D::square(1.f);
		//Ref<Mesh> squaremesh = MeshFactory3D::cubeCorner(1.f);
		squaremesh->Transform(glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, 0.f, 0.f)));
		m_SquareModel->AddMesh(squaremesh);

		m_TriangleModel = MakeRef<LSE::Model>();
		Ref<Mesh> t = MeshFactory2D::triangle45(1.f);
		m_TriangleModel->AddMesh(t);


		RenderCommand::SetClearColour(glm::vec4(0.f, 0.f, 0.f, 1.f));
		RenderCommand::EnableDepthTest(true);
		RenderCommand::EnableFaceCulling(false);
	}

	void drawTwoways(glm::mat4 currentmat = glm::mat4(1.f), float scale = 1.f)
	{
		using namespace LSE;

		if (scale == 1.f) its = 0;

		its++;
		if (scale * scale - minsize < epsilon) return;

		float c = cosf(angle);
		float s = sinf(angle);

		glm::mat4 left;
		glm::mat4 right;

		{

			float c2 = cosf(2.f * angle);
			float s2 = sinf(2.f * angle);
			m_SimpleShader->SetUniform4f("color", glm::mix(glm::vec4(1.f, 0.6f, 0.3f, 1.f), glm::vec4(0.1f, 0.8f, 0.2f, 1.f), (float) 1.f / scale / 30.f));
			Renderer::Submit(m_SimpleShader, m_SquareModel, currentmat);
			currentmat = currentmat * glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, 0.f));

			glm::mat4 tm = glm::mat4(1.f);

			tm[1] = glm::vec4(c2 * 0.5f - 0.5f, s2 * 0.5f, 0.f, 0.f);

			m_SimpleShader->SetUniform4f("color", glm::mix(glm::vec4(1.f, 0.6f, 0.3f, 1.f), glm::vec4(0.1f, 0.8f, 0.2f, 1.f), (float) 1.f / scale / 30.f));
			Renderer::Submit(m_SimpleShader, m_TriangleModel, currentmat * tm);

			glm::mat4 lsc = glm::scale(glm::mat4(1.f), glm::vec3(c, c, 1.f));
			glm::mat4 lro = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 0.f, 1.f));
			glm::mat4 ltr = glm::translate(glm::mat4(1.f), glm::vec3(c2 / 4.f - 0.25f, s2 / 4.f, minsize / scale / scale));

			left = currentmat * ltr * lsc * lro;

			glm::mat4 rsc = glm::scale(glm::mat4(1.f), glm::vec3(s, s, 1.f));
			glm::mat4 rro = glm::rotate(glm::mat4(1.f), angle - glm::pi<float>() / 2.f, glm::vec3(0.f, 0.f, 1.f));
			glm::mat4 rtr = glm::translate(glm::mat4(1.f), glm::vec3(c2 / 4.f + 0.25f, s2 / 4.f, minsize / scale / scale));

			right = currentmat * rtr * rsc * rro;

		}

		drawTwoways(left, scale * c);
		drawTwoways(right, scale * s);
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			//m_CameraController->OnUpdate(delta);
		}

		m_Frames += 1.f;
		m_Time += delta;

		if (m_Time > 1.f)
		{
			m_FPS = m_Frames / m_Time;
			m_Frames = 0.f;
			m_Time = 0.f;
		}

		if (anglechange || sizechange || true)
		{
			RenderCommand::Clear();
			Renderer::BeginScene(m_Camera);
			drawTwoways(glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1.5f, 0.f)));
			Renderer::EndScene();
			anglechange = false;
			sizechange = false;
		}

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::Text((std::string("Its: ") + std::to_string(its)).c_str());
		static bool sac = true;
		bool ac = ImGui::SliderFloat("Angle", &angle, glm::pi<float>() / 4.f - 0.6f, glm::pi<float>() / 4.f + 0.6f);
		if (sac && !ac) anglechange = true;
		sac = ac;

		static bool ssc = true;
		bool sc = ImGui::SliderFloat("MinSize", &minsize, 0.00002f, 1.f, "%.5f", 10.f);
		if (ssc && !sc) sizechange = true;
		ssc = sc;

		ImGui::End();
	}

	void OnEvent(LSE::Event& e) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			//m_CameraController->OnEvent(e);
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

