#pragma once

namespace Engine{

	// Xbox(Microsoft)
	typedef enum class XboxCode : uint16_t
	{
		// From glfw3.h
		ButtonA = 0,
		ButtonB = 1,
		ButtonX = 2,
		ButtonY = 3,
		ButtonLB = 4,
		ButtonRB = 5,
		ButtonBack = 6,
		ButtonStart = 7,
		ButtonGuide = 8,
		ButtonLS = 9,
		ButtonRS = 10,
		ButtonUp = 11,
		ButtonRight = 12,
		ButtonDown = 13,
		ButtonLeft = 14,

		AxisLeftX = 0,
		AxisLeftY = 1,
		AxisRightX = 2,
		AxisRightY = 3,
		AxisLT = 4,
		AxisRT = 5,
		// CUSTOMED parameter for directions(for a axis up to 2)
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
	} Xbox;
	inline std::ostream& operator<<(std::ostream& os, XboxCode xboxCode)
	{
		os << static_cast<int32_t>(xboxCode);
		return os;
	}

	// PlayStation(Sony)
	typedef enum class DSCode : uint16_t
	{
		ButtonCross = 0,
		ButtonCircle = 1,
		ButtonSquare = 2,
		ButtonTriangle = 3,
		ButtonLS = 4,
		ButtonRS = 5,
		ButtonBack = 6,
		ButtonOption = 7,
		ButtonGuide = 8,
		ButtonLT = 9,
		ButtonRT = 10,
		ButtonUp = 11,
		ButtonRight = 12,
		ButtonDown = 13,
		ButtonLeft = 14,
		

		AxisLeftX = 0,
		AxisLeftY = 1,
		AxisRightX = 2,
		AxisRightY = 3,
		AxisLT = 4,
		AxisRT = 5,
	} DS;
	inline std::ostream& operator<<(std::ostream& os, DSCode dsCode)
	{
		os << static_cast<int32_t>(dsCode);
		return os;
	}
}

// From glfw3.h
#define GLFW_GAMEPAD_BUTTON_A               0
#define GLFW_GAMEPAD_BUTTON_B               1
#define GLFW_GAMEPAD_BUTTON_X               2
#define GLFW_GAMEPAD_BUTTON_Y               3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GLFW_GAMEPAD_BUTTON_BACK            6
#define GLFW_GAMEPAD_BUTTON_START           7
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT
// playStation Sticks
#define GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y

// sticks and trigger (linear-controlled)
#define GLFW_GAMEPAD_AXIS_LEFT_X        0
#define GLFW_GAMEPAD_AXIS_LEFT_Y        1
#define GLFW_GAMEPAD_AXIS_RIGHT_X       2
#define GLFW_GAMEPAD_AXIS_RIGHT_Y       3
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GLFW_GAMEPAD_AXIS_LAST          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER