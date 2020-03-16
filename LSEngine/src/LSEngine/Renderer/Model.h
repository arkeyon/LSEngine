#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/Mesh.h"
#include "LSEngine/Renderer/Material.h"
#include "LSEngine/Renderer/Texture.h"
#include "LSEngine/Renderer/VertexArray.h"

#include <vector>

namespace LSE {

	struct Model
	{
		Model();
		void AddMesh(Ref<Mesh> mesh);

		std::vector<Ref<VertexArray>> m_VAOs;
		std::vector<Ref<Mesh>> m_Meshs;
		std::vector<Ref<Material>> m_Materials;
		std::vector<Ref<Texture>> m_Textures;
	};

}