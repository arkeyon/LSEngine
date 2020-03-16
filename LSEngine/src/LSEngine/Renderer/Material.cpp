#include "lsepch.h"
#include "Material.h"

namespace LSE {

	Material::Material()
	{
		static uint32_t id = 0;
		m_ID = id++;
	}

}