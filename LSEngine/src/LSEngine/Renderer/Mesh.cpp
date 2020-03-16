#include "lsepch.h"
#include "Mesh.h"

namespace LSE {

	void Mesh::Invert()
	{
		for (int i = 0; i < m_IndicesCount / (uint32_t)m_Primitive; ++i)
		{
			//for (int n = 0; n < (uint32_t)m_Primitive >> 1; ++n)
			//{
			//	Swap<uint32_t>(m_Indices[i * (uint32_t)m_Primitive + n], m_Indices[(i + 1) * (uint32_t)m_Primitive - n - 1]);
			//}
			Swap<uint32_t>(m_Indices[i * (uint32_t)m_Primitive], m_Indices[i * (uint32_t)m_Primitive + 2]);
		}

		for (int i = 0; i < m_VerticesCount; ++i)
		{
			m_Vertices[i].a_Normal *= -1.f;
		}

		//return MakeRef<Mesh>(this);
	}

}