#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "LSEngine/ECS/Component.h"
#include "LSEngine/ECS/Entity.h"

#include "LSEngine/Renderer/Model.h"

class WorldCollider : public LSE::EntityComponent
{
public:
	float m_StaticFriction;
	float m_DynamicFriction;

	WorldCollider() = default;

	WorldCollider(LSE::Entity* parent, const float& staticfriction, const float& dynamicfriction);

	ENTITYCOMPONENT_TYPE(ENTITYCOMPONENT_WORLDCOLIDER)
};

class Renderable : public LSE::EntityComponent
{
public:
	LSE::Ref<LSE::Model> m_Model;

	Renderable() = default;

	Renderable(LSE::Entity* parent, LSE::Ref<LSE::Model> model);

	ENTITYCOMPONENT_TYPE(ENTITYCOMPONENT_RENDER)

};

class ReferenceFrame : public LSE::EntityComponent
{
public:
	glm::vec3 m_Pos;
	glm::quat m_Orin;
	glm::vec3 m_Scale;

	glm::mat4 getModelMat();

	ReferenceFrame() = default;

	ReferenceFrame(LSE::Entity* parent, glm::vec3 pos, glm::quat orin, glm::vec3 scale);

	ENTITYCOMPONENT_TYPE(ENTITYCOMPONENT_REFERENCEFRAME)
};

class WorldEntity : public LSE::Entity
{
public:
	WorldEntity(glm::vec3 pos, glm::quat orin, glm::vec3 scale, LSE::Ref<LSE::Model> model);
};
