#include "lsepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace LSE {

	RendererAPI* RenderCommand::s_RenderAPI = new OpenGLRendererAPI();

}