#include "lsepch.h"

#include "WindowsTimer.h"

#include <Windows.h>

namespace LSE {

	WindowsTimer::WindowsTimer()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_Frequency = (double)(frequency.QuadPart);
		QueryPerformanceCounter(&m_Start);
	}

	void WindowsTimer::reset()
	{
		QueryPerformanceCounter(&m_Start);
	}

	double WindowsTimer::elapsed() const
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (now.QuadPart - m_Start.QuadPart) / m_Frequency;
	}
	 
	double WindowsTimer::tick() const
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (now.QuadPart - m_LastTick.QuadPart) / m_Frequency;
	}

	Timer* Timer::Create()
	{
		return new WindowsTimer();
	}
}