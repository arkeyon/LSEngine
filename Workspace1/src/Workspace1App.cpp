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

#define tilewidth 25
#define tileheight 25
#define tilesize tilewidth * tileheight

class ExampleLayer : public LSE::Layer
{
private:
	
	struct Tile
	{
		int ID;
		glm::vec3 moveoffs;
		float elapsed;

		glm::vec3 moveoffs2;
		bool combining;

		Tile(int id, glm::vec3 offs1, glm::vec3 offs2)
			: moveoffs(offs1), moveoffs2(offs2), ID(id), elapsed(0.f), combining(true)
		{}

		Tile(int id, glm::vec3 offs, bool comb)
			: moveoffs(offs), moveoffs2(glm::vec3()), ID(id), elapsed(0.f), combining(comb)
		{}

		Tile(int id)
			: moveoffs(glm::vec3()), moveoffs2(glm::vec3()), ID(id), elapsed(0.f), combining(false)
		{}

		Tile()
			: moveoffs(glm::vec3()), moveoffs2(glm::vec3()), ID(0), elapsed(0.f), combining(false)
		{}
	};

	LSE::Ref<LSE::Model> m_TileModel;
	LSE::Ref<LSE::Model> m_BackModel;
	std::array<LSE::Ref<LSE::Texture2D>, 17> m_Tiles;
	std::array<Tile, tilesize> m_TileMap;
	std::array<Tile, tilesize> m_LastTileMap;

	LSE::Ref<LSE::Shader> m_Shader;
	//LSE::Ref<LSE::OrthographicCamera> m_Camera;

	LSE::Ref<LSE::PerspectiveCamera> m_Camera;
	LSE::Ref<LSE::PerspectiveCameraController> m_CameraController;

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
	bool AddTile(int x = -1, int y = -1)
	{
		bool findcoord = (x == -1 || y == -1);
		int tile = x + y * tilewidth;
		if (findcoord)
		{
			std::fill(m_EmptyTiles.begin(), m_EmptyTiles.end(), 0);
			int count = 0;
			for (int i = 0; i < tilesize; i++) if (!m_TileMap[i].ID) m_EmptyTiles[count++] = i;

			if (!count) return true;

			srand(time(NULL));
			tile = m_EmptyTiles[rand() % count];
			srand(time(NULL) + 1);
		}
		m_TileMap[tile] = Tile((rand() % 10) ? 1 : 2, glm::vec3(0.f, 0.f, 5.f), false);
		return false;
	}

	int translateCoords(int x, int y, move_direction dir)
	{
		auto dims = translateDimensions(dir);
		int twidth = dims.first;

		if (dir == left) return x + y * tilewidth;
		if (dir == right) return (twidth - 1 - x) + y * tilewidth;

		if (dir == up) return y + (twidth - 1 - x) * tilewidth;
		if (dir == down) return y + x * tilewidth;
	}

	std::pair<int, int> translateDimensions(move_direction dir)
	{
		if (dir == left || dir == right) return std::pair<int, int>(tilewidth, tileheight);
		if (dir == up || dir == down) return std::pair<int, int>(tileheight, tilewidth);
	}

	glm::mat3 translateDir(move_direction dir)
	{
		glm::mat3 r(0.f);
		r[0][0] = (dir == left) ? 1.f : ((dir == right) ? -1.f : 0.f);
		r[0][1] = (dir == down) ? 1.f : ((dir == up) ? -1.f : 0.f);	
		//r[1][0] = (dir == up || dir == down) ? 1.f :  0.f;
		//r[1][1] = (dir == left || dir == right) ? 1.f : 0.f;
		//r[2][2] = 1.f;
		return r;
	}

	bool MoveTiles(move_direction dir)
	{
		for (Tile& tile : m_TileMap)
		{
			if (tile.elapsed != 0.f) tile = Tile(tile.ID);
		}

		auto ptilemap = m_TileMap;
		glm::mat3 tdir = translateDir(dir);

		bool moved = false;

		auto dims = translateDimensions(dir);
		int twidth = dims.first;
		int theight = dims.second;

		for (int y = 0; y < theight; y++)
		{
			for (int x = 0; x < twidth - 1; x++)
			{
				int xstart = x;
				int ystart = y;
				Tile& tile1 = m_TileMap[translateCoords(xstart, ystart, dir)];
				if (!tile1.ID) continue;

				for (int n = 1; n < twidth - x; n++)
				{
					int xsearch = x + n;
					int ysearch = y;
					Tile& tile2 = m_TileMap[translateCoords(xsearch, ysearch, dir)];
					if (!tile2.ID) continue;

					if (tile1.ID == tile2.ID)
					{
						tile1 = Tile(tile1.ID + 1, tdir * glm::vec3((float)n, 0.f, 0.f), true);
						tile2 = Tile();
						moved = true;
					}

					x += n - 1;
					break;
				}
			}
		}
		
		for (int y = 0; y < theight; y++)
		{
			int zerocount = 0;
			for (int x = 0; x < twidth; x++)
			{
				int& tile = m_TileMap[translateCoords(x, y, dir)].ID;
				if (!tile)
				{
					zerocount++;
					continue;
				}

				if (zerocount)
				{
					Tile& tile1 = m_TileMap[translateCoords(x - zerocount, y, dir)];
					Tile& tile2 = m_TileMap[translateCoords(x, y, dir)];
					glm::vec3 zerodist = tdir * glm::vec3((float)zerocount, 0.f, 0.f);
					if (tile2.combining) tile1 = Tile(tile2.ID, tile2.moveoffs + zerodist, zerodist);
					else tile1 = Tile(tile2.ID, tile2.moveoffs + tdir * glm::vec3((float)zerocount, 0.f, 0.f), false);
					moved = true;
					tile2 = Tile();
				}

			}
		}

		if (!moved) return true;
		m_LastTileMap = ptilemap;
		return AddTile();
	}

	ExampleLayer()
		: Layer("ExampleLayer")
	{
		using namespace LSE;

		RendererAPI::SetAPI(RendererAPI::API::OpenGL);
		Renderer::Init();

		//m_Camera = MakeRef<OrthographicCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 0.f, tilewidth, 0.f, tileheight, -2.f, 5.f);
		m_Camera = MakeRef<PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::two_pi<float>() / 6.f, 16.f / 9.f, 1.f, 100.f);
		m_CameraController = MakeRef<PerspectiveCameraController>(m_Camera);

		m_Shader.reset(Shader::Create("assets/shaders/simpletexshader.glsl"));

		m_TileModel = MakeRef<LSE::Model>();
		m_TileModel->AddMesh(MeshFactory::generateCubeCorner(1.f));

		m_BackModel = MakeRef<LSE::Model>();
		m_BackModel->AddMesh(MeshFactory::generatePlaneCorner(glm::vec3(tilewidth, 0.f, 0.f), glm::vec3(0.f, tileheight, 0.f)));

		for (int i = 0; i < 17; i++)
		{
			m_Tiles[i] = Texture2D::Create(std::string("assets/textures/Base 2/") + std::to_string(i) + std::string(".png"));
		}

		for (int i = 0; i < tilesize; i++)
		{
			m_TileMap[i] = Tile();
			m_LastTileMap[i] = Tile();
		}
		AddTile();

		RenderCommand::SetClearColour(glm::vec4(0.7f, 0.7f, 0.7f, 1.f));
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

		RenderCommand::Clear();

		m_Shader->SetUniformi("tex", 0);
		Renderer::BeginScene(m_Camera);

		for (int y = 0; y < tileheight; y++)
		{
			for (int x = 0; x < tilewidth; x++)
			{
				Tile& tile = m_TileMap[x + y * tilewidth];
				glm::vec3 tilepos = glm::vec3((float)x, (float)y, 0.f);
				glm::vec3 tileanimate1 = tile.moveoffs * (1.f - std::min(tile.elapsed, 1.f));
				glm::vec3 tileanimate2 = tile.moveoffs2 * (1.f - std::min(tile.elapsed, 1.f));
				
				if (tile.combining && glm::length2(tileanimate1 - tileanimate2) != 0.f)
				{
					int ntile = std::max(0, tile.ID - 1);
					if (!ntile) continue;
					m_Tiles[ntile]->Bind(0);

					Renderer::Submit(m_Shader, m_TileModel, glm::translate(glm::mat4(1.f), tilepos + tileanimate1));
					Renderer::Submit(m_Shader, m_TileModel, glm::translate(glm::mat4(1.f), tilepos + tileanimate2));
				}
				else
				{
					if (!tile.ID) continue;
					m_Tiles[tile.ID]->Bind(0);

					Renderer::Submit(m_Shader, m_TileModel, glm::translate(glm::mat4(1.f), tilepos + tileanimate1));
				}

				tile.elapsed += delta * 2.f;
			}
		}

		m_Tiles[0]->Bind(0);
		Renderer::Submit(m_Shader, m_BackModel);

		Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text((std::string("FPS: ") + std::to_string(m_FPS)).c_str());

		if (m_Paused)
		{
			if (ImGui::SliderFloat3("Camera->Pos", (float*)&(m_Camera->GetPos()[0]), -10.f, 10.f))
			{
				auto& pos = m_Camera->GetPos();
				m_Camera->SetPos(glm::vec3(floorf(pos.x), floorf(pos.y), floorf(pos.z)));
			}

			if (ImGui::SliderFloat3("Camera->Angles", (float*)&(m_Camera->GetAngles()[0]), -glm::pi<float>(), glm::pi<float>()))
			{
				auto& ang = m_Camera->GetAngles();
				m_Camera->SetAngles(glm::vec3(floorf(ang.x / glm::quarter_pi<float>()) * glm::quarter_pi<float>(), floorf(ang.y / glm::quarter_pi<float>()) * glm::quarter_pi<float>(), floorf(ang.z / glm::quarter_pi<float>()) * glm::quarter_pi<float>()));
			}
		}
		else
		{
			ImGui::Text((std::string("Camera->Pos: ") + std::to_string(m_Camera->GetPos().x) + ", " + std::to_string(m_Camera->GetPos().y) + ", " + std::to_string(m_Camera->GetPos().z)).c_str());
			ImGui::Text((std::string("Camera->Angles: ") + std::to_string(m_Camera->GetAngles().x) + ", " + std::to_string(m_Camera->GetAngles().y) + ", " + std::to_string(m_Camera->GetAngles().z)).c_str());
		}

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
					for (int i = 0; i < m_TileMap.size(); i++)
					{
						Tile& tile1 = m_TileMap[i];
						Tile& tile2 = m_LastTileMap[i];
						tile1 = Tile(tile2.ID);
					}
					break;
				default:
					break;
				}

				return false;
			});

		EventDispatcher mred(e);
		mred.Dispatch<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& e)
			{
				glm::vec4 mousepos;
				{
					auto mp = LSE::Input::GetMousePos();
					mousepos = glm::vec4(mp.first, mp.second, 0.f, 1.f);
				}

				auto width = LSE::Application::Get().GetWindow().GetWidth();
				auto height = LSE::Application::Get().GetWindow().GetHeight();

				glm::mat4 invscreen = LSE::Maths::ScreenInverse(width, height);
				glm::vec4 nmp = invscreen * mousepos;

				glm::mat4 inverse = glm::inverse(m_Camera->GetVP());

				glm::vec4 pos1 = inverse * glm::vec4(nmp.x, nmp.y, 0.f, 1.f);
				pos1 /= pos1.w;
				glm::vec4 pos2 = inverse * glm::vec4(nmp.x, nmp.y, 1.f, 1.f);
				pos2 /= pos2.w;

				glm::vec4 diff = pos2 - pos1;

				float t = pos1.z / (pos1.z - pos2.z);
				glm::vec4 pos = pos1 + t * diff;

				int x = (int)pos.x;
				int y = (int)pos.y;

				if (x >= 0 && x < tilewidth && y >= 0 && y < tileheight)
				{
					switch (e.GetButtonCode())
					{
					case LSE_MOUSE_BUTTON_1:
						m_TileMap[x + y * tilewidth] = Tile();
						break;
					case LSE_MOUSE_BUTTON_2:
						AddTile((int)pos.x, (int)pos.y);
						break;
					default:
						break;
					}
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

