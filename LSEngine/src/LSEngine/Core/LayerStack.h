#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace LSE {

	class LSE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Ref<Layer>& layer);
		void PushOverlay(Ref<Layer>& overlay);
		void PopLayer(Ref<Layer>& layer);
		void PopOverlay(Ref<Layer>& overlay);

		inline std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		inline std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Ref<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

}