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
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/quaternion.hpp>

#define tilewidth 4
#define tileheight 4
#define tilesize tilewidth * tileheight

class ExampleLayer : public LSE::Layer
{
private:

	LSE::Ref<LSE::Model> m_TileModel;
	std::array<LSE::Ref<LSE::Texture2D>, 20> m_Tiles;

	union TileType {
		TileType() = default;
		TileType(short a, short b)
			: a(a), b(b)
		{}
		int id;
		struct
		{
			short a;
			short b;
		};
	};

	std::array<TileType, tilesize> m_TileMap;
	std::array<TileType, tilesize> m_LastTileMap;

	LSE::Ref<LSE::Shader> m_Shader;
	LSE::Ref<LSE::OrthographicCamera> m_Camera;

	bool m_Paused = false;

	float m_FPS = 0.f;
	float m_Frames = 0.f;
	float m_Time = 0.f;

	std::array<int, tilesize> m_EmptyTiles;

	enum move_direction
	{
		up, down, left, right
	};

public:
	bool AddTile()
	{
		std::fill(m_EmptyTiles.begin(), m_EmptyTiles.end(), 0);
		int count = 0;
		for (int i = 0; i < tilesize; i++) if (!m_TileMap[i].id) m_EmptyTiles[count++] = i;

		if (!count) return true;
		srand(time(NULL));
		int tile = m_EmptyTiles[rand() % count];
		srand(time(NULL) + 1);
		int r = rand() % 10;
		if (r == 0) m_TileMap[tile] = TileType(1, 2);
		else if (r > 6) m_TileMap[tile] = TileType(2, 1);
		else  m_TileMap[tile] = TileType(1, 1);

		return false;
	}

	int translateCoords(int x, int y, move_direction dir)
	{
		if (dir == left) return x + y * tilewidth;
		if (dir == right) return (tilewidth - 1 - x) + y * tilewidth;

		if (dir == up) return y + (tilewidth - 1 - x) * tilewidth;
		if (dir == down) return y + x * tilewidth;
	}

	bool MoveTiles(move_direction dir)
	{
		m_LastTileMap = m_TileMap;

		// for LEFT xdir = -1, ydir = 0

		bool moved = false;

		for (int y = 0; y < tileheight; y++)
		{
			for (int x = 0; x < tilewidth - 1; x++)
			{
				int x1 = x;
				int y1 = y;
				TileType& tile1 = m_TileMap[translateCoords(x1, y1, dir)];
				if (!tile1.id) continue;

				for (int n = 1; n < tilewidth - x; n++)
				{
					int x2 = x + n;
					int& y2 = y;
					TileType& tile2 = m_TileMap[translateCoords(x2, y2, dir)];
					if (!tile2.id) continue;

					if (tile1.b == tile2.b)
					{
						if (tile1.a == tile2.a)
						{
							bool continue2 = false;

							for (int n2 = 1; n2 < tilewidth - x2; n2++)
							{
								int x3 = x2 + n2;
								int& y3 = y2;
								TileType& tile3 = m_TileMap[translateCoords(x3, y3, dir)];
								if (!tile3.id) continue;

								if (tile3.id == tile2.id)
								{
									tile1.b++;
									tile2.id = 0;
									tile3.id = 0;

									moved = true;
									continue2 = true;

									x += n2 + n - 2;
								}

								break;
							}

							if (continue2) continue;
						}

						if (tile1.a + tile2.a <= 3)
						{
							if (tile1.a + tile2.a == 3)
							{
								tile1.a = 1;
								tile1.b++;
							}
							else
							{
								tile1.a = 2;
							}
							tile2.id = 0;
							moved = true;
						}
					}

					x += n - 1;
					break;
				}
			}
		}

		for (int y = 0; y < tileheight; y++)
		{
			int zerocount = 0;
			for (int x = 0; x < tilewidth; x++)
			{
				TileType& tile = m_TileMap[translateCoords(x, y, dir)];
				if (!tile.id)
				{
					zerocount++;
					continue;
				}

				if (zerocount)
				{
					m_TileMap[translateCoords(x - zerocount, y, dir)] = m_TileMap[translateCoords(x, y, dir)];
					moved = true;
					m_TileMap[translateCoords(x, y, dir)].id = 0;
				}

			}
		}

		if (!moved) return true;
		return AddTile();
	}

	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.f, 1.f, 0.f, 1.f, -1.f, 1.f);

		m_Shader.reset(Shader::Create("assets/shaders/simpleshader.glsl"));

		{
			m_TileModel = MakeRef<LSE::Model>();
			Ref<Mesh> tilemesh = MeshFactory::planeCorner(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
			tilemesh->Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 0.f)) * glm::rotate(glm::mat4(1.f), -glm::half_pi<float>(), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(glm::mat4(1.f), glm::vec3(0.9f, 0.9f, 1.f)));
			m_TileModel->AddMesh(tilemesh);
		}

		for (int i = 0; i <= 9; i++)
		{
			for (int n = 0; n <= 1; n++) {
				m_Tiles[i * 2 + n] = Texture2D::Create(std::string("assets/textures/Base 3/") + std::to_string(n + 1) + std::to_string(i) + std::string(".png"));
			}
		}

		for (int i = 0; i < tilesize; i++)
		{
			m_TileMap[i].id = 0;
			m_LastTileMap[i].id = 0;
		}
		AddTile();

		RenderCommand::SetClearColour(glm::vec4(0.7f, 0.7f, 0.7f, 1.f));
	}

	void OnUpdate(float delta) override
	{
		using namespace LSE;

		if (!m_Paused)
		{

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
		Renderer::BeginScene(m_Camera);

		for (int y = 0; y < tileheight; y++)
		{
			for (int x = 0; x < tilewidth; x++)
			{
				TileType& t = m_TileMap[x + y * tilewidth];
				m_Tiles[(t.a == 0 ? 0 : t.a - 1) + t.b * 2]->Bind(0);
				Renderer::Submit(m_Shader, m_TileModel, glm::translate(glm::mat4(1.f), glm::vec3((float)x / tilewidth, (float)y / tileheight, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(1.f / tilewidth, 1.f / tileheight, 1.f)));
			}
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

		}

		EventDispatcher ed(e);
		ed.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e)
			{
				if (e.GetKeyCode() == LSE_KEY_ESCAPE)
				{
					m_Paused = !m_Paused;
					auto& window = Application::Get().GetWindow();
					if (m_Paused) window.SetCursorState(true);
					else window.SetCursorState(false);
				}

				switch (e.GetKeyCode())
				{
				case LSE_KEY_UP:
					MoveTiles(up);
					break;
				case LSE_KEY_DOWN:
					MoveTiles(down);
					break;
				case LSE_KEY_LEFT:
					MoveTiles(left);
					break;
				case LSE_KEY_RIGHT:
					MoveTiles(right);
					break;
				case LSE_KEY_R:
					m_TileMap = m_LastTileMap;
				default:
					break;
				}
				return false;
			});
	}
};

class Sandbox : public LSE::Application
{
public:
	Sandbox()
		:LSE::Application(1280, 720)
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
