#pragma once

#include "LSEngine/Core/Core.h"	
#include "LSEngine/Events/Event.h"

namespace LSE {

	class LSE_API Layer
	{
	private:

		friend LSE::Application;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnActivate() {}
		virtual void OnDeactivate() {}

	public:
		Layer(const std::string& name = "Layer", bool active = true);
		virtual ~Layer();

		virtual void OnUpdate(float delta) {}
		virtual void OnImGuiRender () {}
		virtual void OnEvent(Event& e) {}

		inline bool isActive() const { return m_Active; }
		inline void Activate() { m_Active = true; OnActivate(); }
		inline void Deactivate() { m_Active = false; OnDeactivate(); }

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		const std::string& m_DebugName;

		bool m_Active;
	};

}