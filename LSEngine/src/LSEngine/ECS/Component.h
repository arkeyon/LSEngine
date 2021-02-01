#pragma once

#include "LSEngine/Core/Core.h"

#define ENTITYCOMPONENT_RENDER BIT(0L)
#define ENTITYCOMPONENT_WORLDCOLIDER BIT(1L)
#define ENTITYCOMPONENT_REFERENCEFRAME BIT(2L)

#define ENTITYCOMPONENT_TYPE(type)\
	static const int StaticGetType() { return type; }\
	inline int GetType() const override { return StaticGetType(); }

namespace LSE {

	class Entity;

	class EntityComponent
	{
	public:
		Ref<Entity> m_Parent;
		//int m_Type;

		EntityComponent(Entity* parent);

		virtual int GetType() const = 0;
	};

}