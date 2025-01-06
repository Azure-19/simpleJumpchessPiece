#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Core/Timestep.h"

#include <glm/glm.hpp>

// simple implement without ECS(not built yet)

namespace Engine {

	struct MouseState{
		bool Chosen = false; // mouse hanging
		bool Flip = false;   // set true when Chosen is changed
		bool Picked = false; // just a flag for hacking
	public:
		inline void Reset() { Chosen = false; Flip = false, Picked = false; }
	};
	struct KeyState {
		bool Chosen = false; // virtual cursor of key
		bool Flip = false;   // set true when chosen/Pick set true
		bool Pick = false;   // true when choose again when chosen
	public:
		inline void Reset() { Chosen = false; Flip = false; Pick = false; }
	};
	struct GamepadState {
		bool ShakeFreeFlag = true; // mostly share state with key. Used in update
	public:
		inline void Reset() { ShakeFreeFlag = true; }
	};
	class Button
	{
	protected:
		bool m_Enabled = true;
		bool m_Trigger = false; // for get control specified by layers
		MouseState m_MouseState;
		KeyState m_KeyState;
		GamepadState m_GamepadState;

		std::array<glm::vec2, 2> m_ScreenCoord;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	public:
		// in screen coordinate
		Button(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, bool enable = true);
		virtual ~Button() = default;

		inline std::array<glm::vec2, 2> GetCoord() const { return m_ScreenCoord; }
		inline glm::vec2 GetCenterCoord() const { 
			return { (m_ScreenCoord[0].x + m_ScreenCoord[1].x) / 2, (m_ScreenCoord[0].y + m_ScreenCoord[1].y) / 2, }; 
		}

		inline bool GetEnableState() const { return m_Enabled; }
		inline void SetEnableState(bool state) { m_Enabled = state; }
		// for hack works
		inline bool CheckTrigger() { return m_Trigger; }
		inline void RecordTrigger() { m_Trigger = true; }
		inline void InvalidateTrigger() { m_Trigger = false; }

		// contain implement of capture flip state(so there's no SetMouseFlip)
		virtual bool IsMouseChosen();
		virtual bool IsMousePressed();
		inline bool GetMouseFlipState() const { return m_MouseState.Flip; }
		inline void SetMousePickedState(bool state) { m_MouseState.Picked = state; }
		inline bool GetMousePickedState() const { return m_MouseState.Picked; }

		inline bool GetKeyFlipState() const { return m_KeyState.Flip; }
		inline void SetKeyFlipState(bool state) { m_KeyState.Flip = state; }
		inline bool GetKeyChosenState() { return m_KeyState.Chosen; }
		inline void SetKeyChosenState(bool state) { m_KeyState.Chosen = state; }
		inline bool GetKeyPickState() { return m_KeyState.Pick; }
		inline void SetKeyPickState(bool state) { m_KeyState.Pick = state; }
		inline void InvalidateState() {
			m_KeyState.Reset();
			m_GamepadState.Reset();
		}

		// update state of button without render
		virtual void OnUpdate(Timestep ts = Timestep(0.01667f));
		virtual void OnEvent(Event& e);
		
	};
}

