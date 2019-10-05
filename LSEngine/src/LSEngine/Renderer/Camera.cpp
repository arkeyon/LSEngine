#include <lsepch.h>
#include "Camera.h"

#include <glm/gtx/transform.hpp>

namespace LSE {

	Camera3D::Camera3D(glm::vec3 pos, glm::vec3 angles, float fov, float ar, float znear, float zfar)
		: m_FOV(fov), m_AR(ar), m_ZNear(znear), m_ZFar(zfar)
	{
		m_ProjectionMatrix = glm::perspective(fov, ar, znear, zfar);
	}

}