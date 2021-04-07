#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Renderer/BufferLayout.h"
#include "LSEngine/Renderer/Material.h"

#include <array>

#include <glm/glm.hpp>

namespace LSE {

	enum class RendererPrimitives
	{
		POINTS = 1, LINES = 2, TRIANGLES = 3, QUADS = 4,
	};

	struct vertex_t
	{
		glm::vec3 a_Position;
		glm::vec4 a_Colour;
		glm::vec3 a_Normal;
		glm::vec2 a_UV;
		int a_Tex;
	};

	typedef uint32_t index_t;

	struct Mesh
	{
		Mesh(const int32_t verticesCount, const int32_t indicesCount)
			: m_VerticesCount(verticesCount), m_IndicesCount(indicesCount), m_Vertices(new vertex_t[verticesCount]), m_Indices(new index_t[m_IndicesCount])
		{

		}

		void Invert();
		void Transform(glm::mat4 matrix);
		void Insert(Ref<Mesh> mesh, int vertexoffs, int indexoffs);

		int32_t m_VerticesCount;
		const int32_t m_IndicesCount;

		RendererPrimitives m_Primitive = RendererPrimitives::TRIANGLES;
		Ref<Material> m_Material;

		vertex_t* m_Vertices;
		index_t* m_Indices;
	};

}