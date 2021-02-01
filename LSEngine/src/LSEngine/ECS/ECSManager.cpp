#include "lsepch.h"
#include "ECSManager.h"

namespace LSE {

	ECSManager* ECSManager::s_Singleton = new ECSManager();
	int ECSManager::s_Compcount = 0;

	ECSManager::ECSManager()
	{

	}

	void ECSManager::Init()
	{

	}

	void ECSManager::Destroy()
	{
		//for (Ref<EntityComponent> comp : m_Components)
		//{
		//	if (!comp) continue;
		//}
	}



}
