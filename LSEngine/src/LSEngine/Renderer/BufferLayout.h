#pragma once

#include "LSEngine/Core/Core.h"

#include <string>
#include <vector>

namespace LSE {

	enum class DataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat2x2, Mat2x3, Mat2x4, Mat3x3, Mat3x4, Mat4x4, Bool, Mat2 = Mat2x2, Mat3 = Mat3x3, Mat4 = Mat4x4
	};

	typedef DataType ShaderDataType;
	typedef ShaderDataType SDT;

	static size_t DataTypeSize(SDT type)
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

		LSE_CORE_ASSERT(false, "Unknown  DataType");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		size_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement(DataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(DataTypeSize(type)), Offset(0), Normalized(normalized) {}

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

}