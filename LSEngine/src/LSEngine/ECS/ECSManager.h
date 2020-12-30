#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"

#include <unordered_map>

#define MAX_ENTITIES 64

namespace LSE {

	class ECSManager
	{
	public:
		ECSManager();

		void Init();

		void Destroy();

		template <class T, typename... Args>
		int CreateComponent(Entity* parent, Args... args)
		{
			Ref<T> component = MakeRef<T>(parent, args...);
			static int id = 0;
			auto ptr = std::dynamic_pointer_cast<EntityComponent>(component);
			m_Components[id++] = ptr;
			return id;
		}

		template <class T>
		Ref<T> GetComponent(Entity* parent)
		{
			for (auto p : m_Components)
			{
				Ref<EntityComponent> comp = p.second;
				LSE_CORE_TRACE("{0}", comp);
				if (!comp.get()) continue;
				if (comp->GetType() != T::StaticGetType()) continue;
				if (comp->m_Parent.get() == parent) return std::dynamic_pointer_cast<T>(comp);
			}
			return nullptr;
		}

		template <class T>
		void DeleteComponent(Entity* parent)
		{
			Ref<T> comp = GetComponent<T>(parent);s
		}

		template <class T, typename... Args>
		Ref<T> CreateEntity(Args... args)
		{
			Ref<T> entity = MakeRef<T>(args...);
			static int id = 0;
			m_Entities[id++] = std::dynamic_pointer_cast<Entity>(entity);
			return entity;
		}

		template <class T>
		Ref<T> GetEntity(int id)
		{
			return nullptr;
		}

		template <class T>
		void DeleteEntity(int id)
		{
			m_Entities.erase(id);
		}

		static ECSManager* s_Singleton;

	private:
		std::unordered_map<int, Ref<Entity>> m_Entities;
		std::unordered_map<int, Ref<EntityComponent>> m_Components;
	};

}

#define ECS LSE::ECSManager::s_Singleton
