#pragma once

#include "LSEngine/Core.h"

namespace LSE {

	class LSE_API GraphicsContext
	{
	public:
		virtual void SwapBuffers() = 0;
		virtual void Init() = 0;
	};

}