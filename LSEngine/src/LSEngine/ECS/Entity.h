#pragma once

#include "LSEngine/Core/Core.h"

#include <vector>

#include "ECSManager.h"
#include "Component.h"

namespace LSE {

	class EntityComponent;

	class Entity
	{
	public:
		Entity();
		~Entity();

		template <typename T, typename... Args>
		inline int AddComponent(Args... args) { return ECSManager::s_Singleton->CreateComponent<T>(this, args...); }

		template <class T>
		inline void RemoveComponent() { ECSManager::s_Singleton->DeleteComponent<T>(this); }

		template <class T>
		inline Ref<T> GetComponent() { return ECS->GetComponent<T>(this); }
	};

}