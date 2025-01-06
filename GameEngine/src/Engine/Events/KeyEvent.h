#pragma once

#include "Event.h"

namespace Engine {

	class KeyEvent : public Event
	{
	protected:	//you shouldn't create a KeyEvent object except for those derives from it
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}
		int m_KeyCode;	//for all events' sth in common(what the key is)and enable the inheritances

	public:
		inline int GetKeyCode() const { return m_KeyCode; }		//can let all KeyEvent Class call just one this method

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)	//all keyevents are the same so implement here
	};

	class KeyPressedEvent : public KeyEvent
	{
	private:
		int m_RepeatCount;	//[Active]0/1(if is pressed for long) | times(may for sometimes count);maybe recording time is better
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode),m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << "repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}	//calling the basic class's instructor

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)		//as 3 functions we can't use a ';'
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};

}