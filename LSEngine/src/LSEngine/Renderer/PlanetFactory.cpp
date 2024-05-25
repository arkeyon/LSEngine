#include "lsepch.h"

#include "PlanetFactory.h"

namespace LSE {

	enum class faces
	{
		bottom = 0,
		top = 1,
		right = 2,
		left = 3,
		back = 4,
		front = 5
	};

	float s(float x, float r)
	{
		return 1.f / (1.f + expf(10.f / r * x));
	}
	
	float p(float x, float r)
	{
		return (4.f * x * x) / (r * r) - sqrtf(r)/6.f;
	}
	
	float crat(float x, float r)
	{
		float ss = glm::min(s(x - r / 2.f, r), s(-x - r / 2.f, r));
		return r * glm::min(ss, p(x, r)) / 5.f;
	}

	class PlanetInfo
	{
	public:
		int meshdetail;
		float radius;
		std::vector<int> facestrides;
		Ref<Mesh> mesh;

		void apply(Ref<Mesh> mesh, float lat, float lon, float r)
		{
			using namespace glm;
			//float latr = lat / 180.f * pi<float>();
			//float lonr = lon / 180.f * pi<float>();
			//
			//float x = (tanf(latr) + 1.f) / 2.f;
			//float y = (tanf(lonr) + 1.f) / 2.f;
			//
			//mesh->m_Vertices[facestrides[(const int)faces::front] + (int)(x * (meshdetail - 3)) + (int)(y * (meshdetail - 3)) * (meshdetail - 2)].a_Colour = vec4(1.f, 0.f, 0.f, 1.f);

			vec3 p0(cos(lat) * cos(lon), sin(lat) * cos(lon), sin(lon));

			for (int i = 0; i < mesh->m_VerticesCount; i++)
			{
				vec3 p1 = glm::normalize(mesh->m_Vertices[i].a_Position);

				float dist = atan2(glm::length(glm::cross(p0, p1)), glm::dot(p0, p1));

				if (dist < r)
				{
					mesh->m_Vertices[i].a_Position *= 1.f + crat(dist, r);
				}
			}
		}
	};

	float sphereDist(float lat0, float lon0, float lat1, float lon1)
	{
		return acos(sin(lat0) * sin(lat1) + cos(lat0) * cos(lat1) * cos(lon0 - lon1));
	}

	Ref<Model> PlanetFactory::Planet(float radius)
	{
		using namespace glm;

		PlanetInfo pi;
		pi.meshdetail = 301;
		pi.radius = radius;

		//pi.mesh = MeshFactory::griddedCubeCenter(pi.radius, pi.meshdetail, vec4(1.f, 1.f, 1.f, 1.f), &pi.facestrides);
		pi.mesh = MeshFactory3D::cubeSphere(pi.radius, pi.meshdetail, vec4(1.f, 1.f, 1.f, 1.f), &pi.facestrides);

		srand(time(0));

		for (int i = 0; i < 100; i++)
		{
			float maxs = glm::pi<float>() / 4.f;
			float mins = 0.1f;

			float rn1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float rn2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float rn3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			float lat = 360.f * rn2;
			float lon = 90.f * (2.f * rn3 - 1.f);

			pi.apply(pi.mesh, lat, lon, rn1 * (maxs - mins) + mins);
		}

		Ref<Model> model = MakeRef<Model>();
		model->AddMesh(pi.mesh);
		
		return model;
	}

}