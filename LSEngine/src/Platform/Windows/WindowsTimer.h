#pragma once

#include "LSEngine/Core/Timer.h"

namespace LSE {

	class WindowsTimer : public Timer
	{
	private:
		LARGE_INTEGER m_Start, m_LastTick;
		double m_Frequency;
	public:
		WindowsTimer();

		void reset() override;

		double elapsed() const override;
		double tick() const override;
	};

}