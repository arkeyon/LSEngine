#include "lsepch.h"

#include "ECS.h"

WorldCollider::WorldCollider(LSE::Entity* parent, const float& staticfriction, const float& dynamicfriction)
	: EntityComponent(parent), m_StaticFriction(staticfriction), m_DynamicFriction(dynamicfriction)
{

}

Renderable::Renderable(LSE::Entity* parent, LSE::Ref<LSE::Model> model)
	: EntityComponent(parent), m_Model(model)
{

}

glm::mat4 ReferenceFrame::getModelMat()
{
	return glm::translate(glm::mat4(1.f), m_Pos) * glm::scale(glm::mat4_cast(m_Orin), m_Scale);
}

ReferenceFrame::ReferenceFrame(LSE::Entity* parent, glm::vec3 pos, glm::quat orin, glm::vec3 scale)
	: EntityComponent(parent), m_Pos(pos), m_Orin(orin), m_Scale(scale)
{

}

WorldEntity::WorldEntity(glm::vec3 pos, glm::quat orin, glm::vec3 scale, LSE::Ref<LSE::Model> model)
{
	//AddComponent<WorldCollider>(2.f, 2.f);
	AddComponent<Renderable>(model);
	AddComponent<ReferenceFrame>(pos, orin, scale);
}
