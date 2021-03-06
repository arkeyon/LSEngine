#pragma once

#include "LSEngine/Core/Input.h"

namespace LSE {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) const override;

		virtual bool IsMouseButtonPressedImpl(int button) const override;

		virtual std::pair<float, float> GetMousePosImpl() const override;
		virtual float GetMouseXImpl() const override;
		virtual float GetMouseYImpl() const override;
	};

}