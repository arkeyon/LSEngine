#include "lsepch.h"
#include "OpenGLVertexArray.h"

namespace LSE {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> buffer)
	{
		glBindVertexArray(m_RendererID);
		buffer->Bind();

		const auto& layout = buffer->GetLayout();

		LSE_CORE_ASSERT(layout.GetElements().size(), "Vertex Buffer has no Layout")

		int index = 0;
		for (const auto& e : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, e.GetComponentCount(), ShaderDataTypeGLType(e.Type), e.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)e.Offset);
			index++;
		}

		m_VertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> buffer)
	{
		glBindVertexArray(m_RendererID);
		buffer->Bind();

		m_IndexBuffer = buffer;
	}

	const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

}