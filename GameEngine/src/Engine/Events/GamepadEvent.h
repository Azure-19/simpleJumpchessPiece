#pragma once

#include "Event.h"

namespace Engine {

	// useless: could not set callback in windowsWindow
	class GamepadConnectEvent : public Event
	{
	private:
		int m_JoystickID;
		bool m_JoystickConnectState;
	public:
		GamepadConnectEvent(int joystickID, bool state) 
			: m_JoystickID(joystickID), m_JoystickConnectState(state){}
		inline int GetJoystickID() { return m_JoystickID; }
		inline bool GetJoyStickConnectState() { return m_JoystickConnectState;}
		EVENT_CLASS_TYPE(GamepadConnection);
		EVENT_CLASS_CATEGORY(EventCategoryGamepad);
	};

}