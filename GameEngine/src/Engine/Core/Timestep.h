#pragma once

namespace Engine {
	// measured in seconds by default, m_Time implemented in Application::Run()
	class Timestep
	{
	private:
		float m_Time;
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const			{ return m_Time; }		// type casting into float, use it as a float implicitly

		float GetSeconds() const		{ return m_Time; }
		float GetMilliseconds() const	{ return m_Time * 1000.0f; }
	};
}