#pragma once

#include "LSEngine/Core.h"
#include "Buffer.h"

#include <memory>
#include <vector>

namespace LSE {

	class LSE_API VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> buffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();

	};
}