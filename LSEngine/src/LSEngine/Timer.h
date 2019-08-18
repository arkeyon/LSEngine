#pragma once

namespace LSE {

	class Timer
	{
	public:
		virtual void reset() = 0;
		virtual double elapsed() const = 0;
		virtual double tick() const = 0;

		static Timer* Create();
	};

}