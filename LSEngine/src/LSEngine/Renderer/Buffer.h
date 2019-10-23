#pragma once

#include "LSEngine/Core/Core.h"

namespace LSE {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat2x2, Mat2x3, Mat2x4, Mat3x3, Mat3x4, Mat4x4, Bool, Mat2 = Mat2x2, Mat3 = Mat3x3, Mat4 = Mat4x4
	};

	typedef ShaderDataType SDT;

	static size_t ShaderDataTypeSize(SDT type)
	{
		switch (type)
		{
		case SDT::Float:	return 4;
		case SDT::Float2:	return 4 * 2;
		case SDT::Float3:	return 4 * 3;
		case SDT::Float4:	return 4 * 4;
		case SDT::Int:		return 4;
		case SDT::Int2:		return 4 * 2;
		case SDT::Int3:		return 4 * 3;
		case SDT::Int4:		return 4 * 4;
		case SDT::Mat2x2:	return 4 * 2 * 2;
		case SDT::Mat2x3:	return 4 * 2 * 3;
		case SDT::Mat2x4:	return 4 * 2 * 4;
		case SDT::Mat3x3:	return 4 * 3 * 3;
		case SDT::Mat3x4:	return 4 * 3 * 4;
		case SDT::Mat4x4:	return 4 * 4 * 4;
		case SDT::Bool:		return 1;
		}

		LSE_CORE_ASSERT(false, "Unknown  ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		size_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		size_t GetComponentCount() const
		{
			switch (Type)
			{
			case SDT::Float:	return 1;
			case SDT::Float2:	return 2;
			case SDT::Float3:	return 3;
			case SDT::Float4:	return 4;
			case SDT::Int:		return 1;
			case SDT::Int2:		return 2;
			case SDT::Int3:		return 3;
			case SDT::Int4:		return 4;
			case SDT::Mat2x2:	return 2 * 2;
			case SDT::Mat2x3:	return 2 * 3;
			case SDT::Mat2x4:	return 2 * 4;
			case SDT::Mat3x3:	return 3 * 3;
			case SDT::Mat3x4:	return 3 * 4;
			case SDT::Mat4x4:	return 4 * 4;
			case SDT::Bool:		return 1;
			}

			LSE_CORE_ASSERT(false, "Unknown  ShaderDataType");
			return 0;
		}
	};

	class LSE_API BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const const size_t& GetStride() const { return m_Stride; }

		inline std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		size_t m_Stride;
	};

	class LSE_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(size_t size, void* vertices);
	};

	class LSE_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(size_t count, uint32_t* indices);
	};

}