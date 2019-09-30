#pragma once

#include "LSEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace LSE {

	class LSE_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void SwapBuffers() override;
		void Init() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}