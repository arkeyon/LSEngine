#pragma once

#include "LSEngine/Core/Core.h"

#include "LSEngine/Renderer/MeshFactory.h"
#include "LSEngine/Renderer/Model.h"

namespace LSE {

	class PlanetFactory
	{
	public:
		static Ref<Model> Planet(float size);
	};

}