#pragma once

#include "LSEngine/Core/Core.h"
#include "LSEngine/Core/Layer.h"

namespace LSE {

	class LSE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender () override;

		void Begin();
		void End();
	};

}