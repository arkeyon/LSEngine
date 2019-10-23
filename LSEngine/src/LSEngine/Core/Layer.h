#pragma once

#include "LSEngine/Core/Core.h"	
#include "LSEngine/Events/Event.h"

namespace LSE {

	class LSE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float delta) {}
		virtual void OnImGuiRender () {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		const std::string& m_DebugName;
	};

}