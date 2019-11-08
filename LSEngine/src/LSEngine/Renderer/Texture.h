#pragma once

#include "LSEngine/Core/Core.h"

namespace LSE {

	class Texture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind() const = 0;
	};

	class Texture2D : public Texture
	{

	};
}