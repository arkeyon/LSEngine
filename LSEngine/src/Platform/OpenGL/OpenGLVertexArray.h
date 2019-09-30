#pragma once

#include "LSEngine/Core.h"
#include "LSEngine/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace LSE {

	static size_t ShaderDataTypeGLType(SDT type)
	{
		switch (type)
		{
		case SDT::Float:	return GL_FLOAT;
		case SDT::Float2:	return GL_FLOAT;
		case SDT::Float3:	return GL_FLOAT;
		case SDT::Float4:	return GL_FLOAT;
		case SDT::Int:		return GL_INT;
		case SDT::Int2:		return GL_INT;
		case SDT::Int3:		return GL_INT;
		case SDT::Int4:		return GL_INT;
		case SDT::Mat2x2:	return GL_FLOAT;
		case SDT::Mat2x3:	return GL_FLOAT;
		case SDT::Mat2x4:	return GL_FLOAT;
		case SDT::Mat3x3:	return GL_FLOAT;
		case SDT::Mat3x4:	return GL_FLOAT;
		case SDT::Mat4x4:	return GL_FLOAT;
		case SDT::Bool:		return GL_BOOL;
		}

		LSE_CORE_ASSERT(false, "Unknown  ShaderDataType");
		return 0;
	}

	class LSE_API OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> buffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> buffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
	private:
		GLuint m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}