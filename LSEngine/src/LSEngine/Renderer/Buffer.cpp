#include "lsepch.h"
#include "Buffer.h"

#include "LSEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace LSE {

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements), m_Stride(0)
	{
		for (auto& element : m_Elements)
		{
			element.Offset = m_Stride;
			m_Stride += element.Size;
		}
	}
	VertexBuffer* VertexBuffer::Create(size_t size, void* vertices)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			LSE_CORE_ASSERT(false, "Cant create VertexBuffer without RenderingAPI");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(size, vertices);
		}
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(size_t count, uint32_t* indices)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			LSE_CORE_ASSERT(false, "Cant create IndexBuffer without RenderingAPI");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(count, indices);
		}
		return nullptr;
	}
}