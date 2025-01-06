#include "egpch.h"
#include "Engine/Core/Input.h"

//only used in <windows> cpp so it's fine for now
#include <GLFW/glfw3.h>
#include "Engine/Core/Application.h"

namespace Engine {

	bool Input::IsKeyPressed(KeyCode key)
	{
		//to get GLFWwindow* from Window(which return void*)
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));		//ask glfw return if the keycode is pressed

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyReleased(KeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));

		return state == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	bool Engine::Input::IsMouseButtonReleased(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

		return state == GLFW_RELEASE;
	}

	void Input::SetMousePosition(float posX, float posY)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetCursorPos(window, posX, posY);
	}

	void Input::SetCursorHiddenState(bool state)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		if (state)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}

	bool Input::GetCursorHiddenState()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);		//parameter are pointers
		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		auto[x, y] = GetMousePosition();		//sth new and cool in c++17
		return x;
	}

	float Input::GetMouseY()
	{
		auto[x, y] = GetMousePosition();
		return y;
	}



	// ready for saving a gamepadState
	GLFWgamepadstate state;

	constexpr float InputTrigger = 10E-3;
	constexpr float buttonTrigger = 0.3f;

	bool Input::IsXboxConnected()
	{
		// for DEFAULET: only one device
		return glfwJoystickIsGamepad(GLFW_JOYSTICK_1) == GLFW_TRUE;
	}

	bool Input::IsGamepadButtonPressed(XboxCode button)
	{
		if (IsXboxConnected())
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			return state.buttons[static_cast<int32_t>(button)];
		}
		return false;
	}

	bool Input::IsGamepadButtonReleased(XboxCode button)
	{
		if (IsXboxConnected())
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			return !state.buttons[static_cast<int32_t>(button)];
		}
		return true;
	}
	// Y-direction in glfw is reversed as screen coordinates
	bool Input::IsGamepadAxisPulled(XboxCode axis, XboxCode direction)
	{
		if (IsXboxConnected)
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			if (axis == XboxCode::AxisLeftY || axis == XboxCode::AxisRightY)
			{
				if (static_cast<int32_t>(direction) ? state.axes[static_cast<int32_t>(axis)] > buttonTrigger :
					state.axes[static_cast<int32_t>(axis)] < -buttonTrigger)
					return true;
			}
			else
			{
				if (static_cast<int32_t>(direction) - 2 ? state.axes[static_cast<int32_t>(axis)] > buttonTrigger:
					state.axes[static_cast<int32_t>(axis)] < -buttonTrigger)
					return true;
			}
		}
		return false;
	}

	bool Input::IsGamepadAxisReset(XboxCode axis, XboxCode direction)
	{
		if (IsXboxConnected)
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			if (axis == XboxCode::AxisLeftY || axis == XboxCode::AxisRightY)
			{
				if (static_cast<int32_t>(direction) ? state.axes[static_cast<int32_t>(axis)] < buttonTrigger :
				state.axes[static_cast<int32_t>(axis)] > -buttonTrigger)
					return true;
			}
			else
			{
				if (static_cast<int32_t>(direction) - 2 ? state.axes[static_cast<int32_t>(axis)] < buttonTrigger:
				state.axes[static_cast<int32_t>(axis)] > -buttonTrigger)
					return true;
			}
		}
		return true;
	}

	bool Input::IsGamepadInput()
	{
		if (IsXboxConnected)
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			for(int i = 0; i<15; i++)
			{
				if (state.buttons[i])
					return true;
			}
			if (Engine::Input::GetGamepadLSPosition() != std::pair<float, float>(-10, -10))
			{
				return true;
			}
			else if (Engine::Input::GetGamepadRSPosition() != std::pair<float, float>(-10, -10))
				return true;
			else if (Engine::Input::GetGamepadLTPosition() != -1)
				return true;
			else if (Engine::Input::GetGamepadRTPosition() != -1)
				return true;
		}
		return false;
	}


	std::pair<float, float> Input::GetGamepadLSPosition()
	{
		if(IsXboxConnected())
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			// filter the offset of sticks(great amount of calculation)
			return
				(pow(state.axes[static_cast<int32_t>(XboxCode::AxisLeftX)], 2) +
				pow(state.axes[static_cast<int32_t>(XboxCode::AxisLeftY)], 2))
					> InputTrigger ?
				std::pair<float, float>(state.axes[static_cast<int32_t>(XboxCode::AxisLeftX)],
					state.axes[static_cast<int32_t>(XboxCode::AxisLeftY)]) :
				std::pair(-10, -10);
		}
		return { -10, -10 };
	}

	std::pair<float, float> Input::GetGamepadRSPosition()
	{
		if (IsXboxConnected())
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			// filter the offset of sticks
			return 
				(pow(state.axes[static_cast<int32_t>(XboxCode::AxisRightX)], 2) +
				 pow(state.axes[static_cast<int32_t>(XboxCode::AxisRightY)], 2))
					> InputTrigger ?
				std::pair<float, float>(state.axes[static_cast<int32_t>(XboxCode::AxisRightX)],
					state.axes[static_cast<int32_t>(XboxCode::AxisRightY)]) : 
				std::pair(-10, -10);
		}
		return { -10, -10 };
	}
	// range from -1 to 1
	float Input::GetGamepadLTPosition()
	{
		if (IsXboxConnected())
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			return state.axes[static_cast<int32_t>(XboxCode::AxisLT)];
		}
		return -1;
	}

	float Input::GetGamepadRTPosition()
	{
		if (IsXboxConnected())
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
			return state.axes[static_cast<int32_t>(XboxCode::AxisRT)];
		}
		return -1;
	}

}