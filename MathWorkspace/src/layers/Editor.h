#pragma once

#include <LSEngine.h>

#include "LSEngine/Events/Event.h"
#include "LSEngine/Core/Core.h"

class EditorLayer : public LSE::Layer
{
public:
	EditorLayer();

	void OnUpdate(float delta) override;
	void OnImGuiRender() override;
	void OnEvent(LSE::Event& e) override;
};