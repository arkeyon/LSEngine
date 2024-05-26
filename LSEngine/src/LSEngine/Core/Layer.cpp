#include "lsepch.h"
#include "Layer.h"

namespace LSE {

	Layer::Layer(const std::string& debugName, bool active)
		: m_DebugName(debugName), m_Active(active)
	{

	}

	Layer::~Layer()
	{

	}
}