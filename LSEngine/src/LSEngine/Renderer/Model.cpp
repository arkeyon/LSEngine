#include "lsepch.h"
#include "Model.h"

namespace LSE {

	Model::Model()
	{

	}

	void Model::AddMesh(Ref<Mesh> mesh)
	{
		Ref<VertexArray> va = VertexArray::Create();
		va->Bind();

		BufferLayout bl = {
				{ SDT::Float3, "a_Position" },
				{ SDT::Float4, "a_Colour" },
				{ SDT::Float3, "a_Normal" },
				{ SDT::Float2, "a_UV" },
				{ SDT::Float, "a_Tex" }
		};

		Ref<VertexBuffer> vertexBuffer(VertexBuffer::Create(mesh->m_VerticesCount * sizeof(vertex_t), mesh->m_Vertices));
		vertexBuffer->SetLayout(bl);

		Ref<IndexBuffer> indexBuffer(IndexBuffer::Create(mesh->m_IndicesCount, mesh->m_Indices));

		va->AddVertexBuffer(vertexBuffer);
		va->SetIndexBuffer(indexBuffer);

		m_VAOs.push_back(va);
		m_Meshs.push_back(mesh);
	}

}