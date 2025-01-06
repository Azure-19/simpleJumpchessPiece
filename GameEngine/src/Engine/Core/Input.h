#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"
#include "Engine/Core/GamepadCodes.h"

namespace Engine {

	// pure Interface, implemented by windowsInput
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsKeyReleased(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static bool IsMouseButtonReleased(MouseCode button);

		// only Xbox code supported now
		static bool IsGamepadButtonPressed(XboxCode button);
		static bool IsGamepadButtonReleased(XboxCode button);
		static bool IsGamepadAxisPulled(XboxCode axis, XboxCode direction);
		static bool IsGamepadAxisReset(XboxCode axis, XboxCode direction);
		static bool IsGamepadInput();

		static void SetMousePosition(float posX, float posY);
		static void SetCursorHiddenState(bool state);// true for hide
		static bool GetCursorHiddenState();
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		// Attention: though not connected I will return true for Released and Reset
		static bool IsXboxConnected();
		static std::pair<float, float> GetGamepadLSPosition();
		static std::pair<float, float> GetGamepadRSPosition();
		static float GetGamepadLTPosition();
		static float GetGamepadRTPosition();
	};

}