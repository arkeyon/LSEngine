#include "lsepch.h"
#include "ECSManager.h"

namespace LSE {

	ECSManager* ECSManager::s_Singleton = new ECSManager();

	ECSManager::ECSManager()
	{

	}

	void ECSManager::Init()
	{

	}

	void ECSManager::Destroy()
	{
		for (std::pair<const int, Ref<EntityComponent>> p : m_Components)
		{
			Ref<EntityComponent> comp = p.second;
			if (!comp) continue;
		}
	}

}
