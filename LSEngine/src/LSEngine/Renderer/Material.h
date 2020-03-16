#pragma once

#include "LSEngine/Renderer/Shader.h"
#include "LSEngine/Renderer/Texture.h"
#include <vector>

namespace LSE {

	class Material
	{
	public:
		Material();
	private:
		uint32_t m_ID;
		Ref<Shader> m_Shader;
		std::vector<Ref<Texture>> m_Textures;
	};

}