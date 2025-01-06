#pragma once

#include "egpch.h"
#include "Engine/Core/Core.h"	//can't use dir related to src don't know why, changing the .vcxproj works

namespace Engine {

	//Events now are currently blocking, no buffer included

	enum class EventType
	{
		None = 0,
		//application event
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		//key event
		KeyPressed, KeyReleased, KeyTyped,
		//mouse event
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		//gamepad event
		GamepadConnection
	};

	enum EventCategory
	{
	// in case we just want some kind of events,like type_traits in STL
	// use bit field and can set multiple categories that some contain-contained relations exist
	// for example 11010 for a MouseButtonClick event and 00110 for any keyboardInput event, just like flags
		None = 0,
		EventCategoryApplication		= BIT(0),
		EventCategoryInput				= BIT(1),
		EventCategoryKeyboard			= BIT(2),
		EventCategoryMouse				= BIT(3),
		EventCategoryMouseButton		= BIT(4),
		EventCategoryGamepad			= BIT(5),
	};


#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }





	class Event
	{
	public:
		bool Handled = false;
		
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); };

		inline bool IsInCategory(EventCategory category)	//filter unwanted events
		{
			return GetCategoryFlags() & category;		//by bit
		}
	};

	class EventDispatcher		//dispatch event base on their types
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;	//refers to any type, to achieve automatic use;;Fn refers to function
	private:
		Event& m_Event;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())		//if match, than handle it
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	};
	
	//-[WARNING]-sth wrong here: EG_TRACE(e) couldn't work but EG_TRACE(e.ToString()) is OK,,maybe fmt doesn't work
	//std::cout << e << std::endl; print only name(solved);;;
	//------------NOW I CONSIDERED SDPLIB HAVE STH WRONG WITH IT SO KEEP USING THE METHOD INSTEAD-------------
	inline std::ostream& operator<<(std::ostream& os, const Event& e)//a reload that may be friendly for use
	{
		return os << e.ToString();
	}

}