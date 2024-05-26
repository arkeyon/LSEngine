
#include "lsepch.h"

#include "Engine.h"

EngineLayer::EngineLayer()
	: Layer("EngineLayer")
{
	using namespace LSE;
	using namespace LSE::Maths;

	RendererAPI::SetAPI(RendererAPI::API::OpenGL);
	Renderer::Init();

	m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), -10.f * 16.f / 9.f, 10.f * 16.f / 9.f, -10.f, 10.f, -1.f, 1.f);
	m_CameraController = MakeRef<OrthographicCameraController>(m_Camera);

	m_Shader.reset(Shader::Create("assets/shaders/simpleshader.glsl"));

	m_Mark = MakeRef<Model>();
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

	auto& window = Application::Get().GetWindow();
	window.SetCursorState(false);

	RenderCommand::EnableFaceCulling(false);
	RenderCommand::EnableDepthTest(true);
	RenderCommand::SetClearColour(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
}

void EngineLayer::OnUpdate(float delta)
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

	std::array<Ref<ParametricCurve>, 5> curves =
	{
		m_Parabola1,
		m_Parabola2,
		m_Line1,
		m_Line2,
		m_UCurve
	};

	std::vector<glm::vec3> solutions;

	for (int i = 0; i < curves.size() - 1; i++)
	{
		auto curve1 = curves[i];
		for (int j = i + 1; j < curves.size(); j++)
		{
			auto curve2 = curves[j];

			auto s = intercept(curve1, curve2);
			solutions.insert(solutions.end(), s.begin(), s.end());
		}
	}

	for (auto s : solutions)
	{
		Renderer::Submit(m_Shader, m_Mark, glm::translate(glm::mat4(1.f), s));
	}

	for (auto c : curves)
	{
		Renderer::Submit(m_Shader, c->GetComponent<Renderable>()->m_Model, c->GetComponent<ReferenceFrame>()->getModelMat());
	}

	Renderer::EndScene();
}

void EngineLayer::OnImGuiRender()
{
	ImGui::Begin("Debug");
	ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());
	ImGui::Text((std::string("Camera->Pos: ") + std::to_string(m_Camera->GetPos().x) + ", " + std::to_string(m_Camera->GetPos().y) + ", " + std::to_string(m_Camera->GetPos().z)).c_str());
	ImGui::Text((std::string("Camera->Angles: ") + std::to_string(m_Camera->GetAngles().x) + ", " + std::to_string(m_Camera->GetAngles().y) + ", " + std::to_string(m_Camera->GetAngles().z)).c_str());
	ImGui::End();
}

void EngineLayer::OnEvent(LSE::Event& e)
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