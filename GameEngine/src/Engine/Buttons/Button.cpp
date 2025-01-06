#include "egpch.h"
#include "Button.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"


namespace Engine {

	bool Button::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (this->IsMousePressed())
		{
			m_MouseState.Picked = true;
			return true;
		}
		return false;
	}

	bool Button::OnMouseMoved(MouseMovedEvent& e)
	{
		if (!m_MouseState.Chosen)
			m_KeyState.Reset();
		return false;
	}

	bool Button::OnKeyPressed(KeyPressedEvent& e)
	{
		if (Input::IsKeyPressed(KeyCode::Z) && m_KeyState.Chosen)
		{
			m_KeyState.Reset();
			m_KeyState.Pick = true;
			return true;
		}
		if (Input::IsKeyPressed(KeyCode::X) && m_KeyState.Chosen)
		{
			m_KeyState.Chosen = false;
			return true;
		}

		return false;
	}

	Button::Button(const glm::vec2& leftLowCoord, const glm::vec2& rightHighCoord, bool enable)
	{
		m_ScreenCoord[0] = leftLowCoord;
		m_ScreenCoord[1] = rightHighCoord;
		m_Enabled = enable;
	}

	bool Button::IsMouseChosen()
	{
		if (Engine::Input::GetCursorHiddenState())
			return false;
		else {
			if ((Input::GetMouseX() > m_ScreenCoord[0].x) && (Input::GetMouseX() < m_ScreenCoord[1].x) &&
				(Input::GetMouseY() < m_ScreenCoord[0].y) && (Input::GetMouseY() > m_ScreenCoord[1].y) &&
				m_Enabled)
			{
				if (!m_MouseState.Chosen)
					m_MouseState.Flip = true;
				else
					m_MouseState.Flip = false;

				m_MouseState.Chosen = true;
				return true;
			}
			else
			{
				if (m_MouseState.Chosen)
					m_MouseState.Flip = true;
				else
					m_MouseState.Flip = false;

				m_MouseState.Chosen = false;
				return false;
			}
		}
	}

	bool Button::IsMousePressed()
	{
		return Input::IsMouseButtonPressed(MouseCode::ButtonLeft) && IsMouseChosen();
	}

	void Button::OnUpdate(Timestep ts)
	{
		if (Input::IsGamepadButtonPressed(XboxCode::ButtonA) && m_KeyState.Chosen)
		{
			if(m_GamepadState.ShakeFreeFlag)
			{
				m_KeyState.Reset();
				m_KeyState.Pick = true;
			}
		}
		else if (Input::IsGamepadButtonReleased(XboxCode::ButtonA))
		{
			if (Input::IsGamepadButtonPressed(XboxCode::ButtonB) && m_KeyState.Chosen)
			{
				if (m_GamepadState.ShakeFreeFlag)
					m_KeyState.Chosen = false;
			}
			else if (Input::IsGamepadButtonReleased(XboxCode::ButtonB))
				m_GamepadState.ShakeFreeFlag = true;
		}

	}

	void Button::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(Button::OnKeyPressed));
		dispatcher.Dispatch<MouseMovedEvent>(EG_BIND_EVENT_FN(Button::OnMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Button::OnMouseButtonPressed));
	}

}
