#include "lsepch.h"
#include "LayerStack.h"

namespace LSE {

	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		for (auto it = m_Layers.begin(); it != m_Layers.end(); it++)
		{
			(*it)->OnDetach();
		}
		m_Layers.clear();
		m_LayerInsertIndex = 0;
	}

	void LayerStack::PushLayer(Ref<Layer>& layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Ref<Layer>& overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Ref<Layer>& layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			(*it)->OnDetach();
			m_Layers.erase(it);
			m_Layers.shrink_to_fit();
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Ref<Layer>& overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			(*it)->OnDetach();
			m_Layers.erase(it);
		}
	}
}