#include "lsepch.h"
#include "Mesh.h"

namespace LSE {

	void Mesh::Invert()
	{

		int verts;

		switch (m_Primitive)
		{
		case RendererPrimitives::TRIANGLES:
			verts = 3;
			break;
		case RendererPrimitives::QUADS:
			verts = 4;
		}

		for (int i = 0; i < m_IndicesCount / verts; ++i)
		{
			//for (int n = 0; n < (uint32_t)m_Primitive >> 1; ++n)
			//{
			//	Swap<uint32_t>(m_Indices[i * (uint32_t)m_Primitive + n], m_Indices[(i + 1) * (uint32_t)m_Primitive - n - 1]);
			//}
			Swap<uint32_t>(m_Indices[i * verts], m_Indices[i * verts + 2]);
		}

		for (int i = 0; i < m_VerticesCount; ++i)
		{
			m_Vertices[i].a_Normal *= -1.f;
		}

		//return MakeRef<Mesh>(this);
	}

	void Mesh::Transform(glm::mat4 matrix)
	{
		for (int i = 0; i < m_VerticesCount; ++i)
		{
			m_Vertices[i].a_Position = glm::vec3(matrix * glm::vec4(m_Vertices[i].a_Position, 1.f));
			m_Vertices[i].a_Normal = glm::vec3(matrix * glm::vec4(m_Vertices[i].a_Normal, 0.f));
		}
	}

	void Mesh::Insert(Ref<Mesh> mesh, int vertexoffs, int indexoffs)
	{
		if (mesh->m_VerticesCount) memcpy(&(m_Vertices[vertexoffs]), mesh->m_Vertices, sizeof(vertex_t) * mesh->m_VerticesCount);
		if (mesh->m_IndicesCount) memcpy(&(m_Indices[indexoffs]), mesh->m_Indices, sizeof(index_t) * mesh->m_IndicesCount);
		for (int i = 0; i < mesh->m_IndicesCount; i++) { m_Indices[i + indexoffs] += vertexoffs; }
	}
}