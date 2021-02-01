#pragma once

#include "LSEngine/Core/Core.h"

#include "Entity.h"
#include "Component.h"
#include "System.h"

#define MAX_ENTITIES 10

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
			auto ptr = std::dynamic_pointer_cast<EntityComponent>(component);
			m_Components[s_Compcount++] = ptr;
			return s_Compcount;
		}

		template <class T>
		Ref<T> GetComponent(Entity* parent)
		{
			for (auto comp : m_Components)
			{
				if (!comp) continue;
				if (comp->GetType() != T::StaticGetType()) continue;
				if (comp->m_Parent.get() == parent) return std::dynamic_pointer_cast<T>(comp);
			}
			return Ref<T>((T*)nullptr);
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
		std::array<Ref<Entity>, MAX_ENTITIES> m_Entities;
		std::array<Ref<EntityComponent>, 100> m_Components;

		static int s_Compcount;
	};

}

#define ECS LSE::ECSManager::s_Singleton
