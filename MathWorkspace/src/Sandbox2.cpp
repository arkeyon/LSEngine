
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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtc/quaternion.hpp>

#include <list>
#include <math.h>

#include "LSEngine/ECS/Entity.h"
#include "LSEngine/ECS/Objects/ECS.h"

#include "LSEngine/Maths/LSEMath.h"

#include "LSEngine/Maths/Curve.h"

int sides = 3;
float scale = 4.f;
float x = 1.f / 2.f;
float y = (1.f - x) / 2.f;

float L = 2.f * sinf(glm::pi<float>() / sides);
//float L = 1.f;

glm::mat4 x2 = glm::scale(glm::mat4(1.f), glm::vec3(x, x, 1.f));
glm::mat4 y2 = glm::scale(glm::mat4(1.f), glm::vec3(y, y, 1.f));
glm::mat4 xt = glm::translate(glm::mat4(1.f), glm::vec3(cosf(glm::pi<float>() / sides) * (1.f + x), 0.f, 0.f));

glm::mat4 yt1 = glm::translate(glm::mat4(1.f), glm::vec3(cosf(glm::pi<float>() / sides) * (1.f + x * y), L * (x + y) / 2.f, 0.f));
glm::mat4 yt2 = glm::translate(glm::mat4(1.f), glm::vec3(cosf(glm::pi<float>() / sides) * (1.f + x * y), -L * (x + y) / 2.f, 0.f));
glm::mat4 flip = { {-1.f, 0.f, 0.f, 0.f},{0.f, 1.f, 0.f, 0.f},{0.f, 0.f, 1.f, 0.f},{0.f, 0.f, 0.f, 1.f} };

int maxdepth = 5;

glm::mat4 m_Scaler = glm::scale(glm::mat4(1.f), glm::vec3(4.f, 4.f, 1.f));

LSE::Ref<LSE::Model> m_RegularPolygon;

class ExampleLayer : public LSE::Layer
{
private:
	LSE::Ref<LSE::Maths::Parabola> m_Parabola1;
	LSE::Ref<LSE::Maths::Parabola> m_Parabola2;
	LSE::Ref<LSE::Maths::Line> m_Line1;
	LSE::Ref<LSE::Maths::Line> m_Line2;

	LSE::Ref<LSE::Maths::UnnamedCurve> m_UCurve;
	LSE::Ref<LSE::Model> m_Mark;

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::OrthographicCamera> m_Camera;
	LSE::Ref<LSE::OrthographicCameraController> m_CameraController;

	bool m_Paused = false;

	LSE::Ref<LSE::Texture2D> m_TestTexture;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;
		using namespace LSE::Maths;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), -10.f * 16.f / 9.f, 10.f * 16.f / 9.f, -10.f, 10.f, -1.f, 1.f);
		m_CameraController = MakeRef<OrthographicCameraController>(m_Camera);

		m_Shader.reset(Shader::Create("assets/shaders/simpleshader.glsl"));

		m_Mark = MakeRef<Model>();
		m_RegularPolygon = MakeRef<Model>();
		//m_RegularPolygon->AddMesh(MeshFactory::planeCenter(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)));
		m_RegularPolygon->AddMesh(MeshFactory::regularPolygon(sides));

		m_Mark->AddMesh(MeshFactory::mark());

		m_Parabola1 = MakeRef<Parabola>(glm::vec3(0.f, -7.f, 0.f), glm::angleAxis(0.2f, glm::vec3(0.f, 0.f, 1.f)), glm::vec3(1.f, 1.f, 1.f), -5.f, 5.f);
		m_Parabola2 = MakeRef<Parabola>(glm::vec3(-6.f, -2.f, 0.f), glm::angleAxis(-1.1f, glm::vec3(0.f, 0.f, 1.f)), glm::vec3(2.f, 1.f, 1.f), -10.f, 5.f);

		m_Line1 = MakeRef<Line>(glm::vec3(2.f, -5.f, 0.f), glm::angleAxis(0.6f, glm::vec3(0.f, 0.f, 1.f)), glm::vec3(1.f, 1.f, 1.f));
		m_Line2 = MakeRef<Line>(glm::vec3(-3.f, -4.f, 0.f), glm::angleAxis(-0.1f, glm::vec3(0.f, 0.f, 1.f)), glm::vec3(2.f, 1.f, 1.f));

		m_UCurve = MakeRef<UnnamedCurve>(glm::vec3(0.f, 0.f, 0.f), glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f)), glm::vec3(4.f, 4.f, 1.f),
			[](const float& t)
			{
				return glm::vec3(t, 1.f - exp(-t), 0.f);
			}, 0.f, 10.f);

		m_TestTexture = Texture2D::Create("assets/textures/BLANK.png");

		flip = glm::rotate(glm::mat4(1.f), glm::pi<float>() * (1.f) / sides, glm::vec3(0.f, 0.f, 1.f)) * flip;

		auto& window = Application::Get().GetWindow();
		window.SetCursorState(false);

		RenderCommand::EnableFaceCulling(false);
		RenderCommand::EnableDepthTest(true);
		RenderCommand::SetClearColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
	}

	void Iterate(const glm::mat4& transform, int depth = 0)
	{
		using namespace LSE;
		using namespace LSE::Maths;

		if (depth >= maxdepth) return;

		for (int i = 0; i < sides - 1; i++)
		{
			glm::mat4 transform_centre = transform * glm::rotate(glm::mat4(1.f), (float)(i + 1.5f) / sides * glm::two_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * xt * flip * x2;
			Renderer::Submit(m_Shader, m_RegularPolygon, m_Scaler * transform_centre);
			Iterate(transform_centre, depth + 1);

			glm::mat4 transform_left = transform * glm::rotate(glm::mat4(1.f), (float)(i + 1.5f) / sides * glm::two_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * yt1 * flip * y2 * x2;
			Renderer::Submit(m_Shader, m_RegularPolygon, m_Scaler * transform_left);
			Iterate(transform_left, depth + 1);

			glm::mat4 transform_right = transform * glm::rotate(glm::mat4(1.f), (float)(i + 1.5f) / sides * glm::two_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * yt2 * flip * y2 * x2;
			Renderer::Submit(m_Shader, m_RegularPolygon, m_Scaler * transform_right);
			Iterate(transform_right, depth + 1);
		}
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;
		using namespace LSE::Maths;

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

		RenderCommand::Clear();

		m_Shader->SetUniformi("tex", 0);
		m_TestTexture->Bind(0);

		Renderer::BeginScene(m_Camera);

		//m_Parabola->intercept((const LSE::Maths::ParametricCurve&)m_Line);

		//std::array<Ref<ParametricCurve>, 4> curves =
		//{
		//	m_Parabola1,
		//	m_Parabola2,
		//	m_Line1,
		//	m_Line2,
		//	//m_UCurve
		//};
		//
		//std::vector<glm::vec3> solutions;
		//
		//for (int i = 0; i < curves.size() - 1; i++)
		//{
		//	auto curve1 = curves[i];
		//	for (int j = i + 1; j < curves.size(); j++)
		//	{
		//		auto curve2 = curves[j];
		//
		//		auto s = intercept(curve1, curve2);
		//		solutions.insert(solutions.end(), s.begin(), s.end());
		//	}
		//}
		//
		//for (auto s : solutions)
		//{
		//	Renderer::Submit(m_Shader, m_Mark, glm::translate(glm::mat4(1.f), s));
		//}
		//
		//for (auto c : curves)
		//{
		//	Renderer::Submit(m_Shader, c->GetComponent<Renderable>()->m_Model, c->GetComponent<ReferenceFrame>()->getModelMat());
		//}

		Renderer::Submit(m_Shader, m_RegularPolygon, m_Scaler);

		for (int i = 0; i < sides; i++)
		{
			glm::mat4 transform_centre = glm::rotate(glm::mat4(1.f), (float)(i + 0.5f) / sides * glm::two_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * xt * flip * x2;
			Renderer::Submit(m_Shader, m_RegularPolygon, m_Scaler * transform_centre);
			Iterate(transform_centre);

			glm::mat4 transform_left = glm::rotate(glm::mat4(1.f), (float)(i + 0.5f) / sides * glm::two_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * yt1 * flip * y2 * x2;
			Renderer::Submit(m_Shader, m_RegularPolygon, m_Scaler * transform_left);
			Iterate(transform_left);
			
			glm::mat4 transform_right = glm::rotate(glm::mat4(1.f), (float)(i + 0.5f) / sides * glm::two_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * yt2 * flip * y2 * x2;
			Renderer::Submit(m_Shader, m_RegularPolygon, m_Scaler * transform_right);
			Iterate(transform_right);
		}

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
		ImGui::Text((std::string("Camera->Pos: ") + std::to_string(m_Camera->GetPos().x) + ", " + std::to_string(m_Camera->GetPos().y) + ", " + std::to_string(m_Camera->GetPos().z)).c_str());
		ImGui::Text((std::string("Camera->Angles: ") + std::to_string(m_Camera->GetAngles().x) + ", " + std::to_string(m_Camera->GetAngles().y) + ", " + std::to_string(m_Camera->GetAngles().z)).c_str());
		ImGui::End();
	}

	void OnEvent(LSE::Event& e) override
	{
		using namespace LSE;

		if (!m_Paused)
		{
			m_CameraController->OnEvent(e);
		}

		EventDispatcher ed(e);
		ed.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
			{
				if (((KeyPressedEvent&)e).GetKeyCode() == LSE_KEY_ESCAPE)
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
		: Application(1920, 1080)
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
