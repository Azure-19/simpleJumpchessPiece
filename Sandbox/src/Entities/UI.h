#pragma once

#include <Engine.h>

struct UIData
{
	float TimeLeft = 9.999f;
	uint32_t Round = 0;
	uint32_t TeamCount = 0;
	uint32_t ActiveTeam = 0;
};

class GameUI
{
private:
	Engine::Ref<Engine::Texture2D> m_Texture,
		m_TintQuad, m_LetterSheet;
	std::unordered_map<char, Engine::Ref<Engine::SubTexture2D>> s_LetterMap;
	UIData m_Data;
public:
	GameUI() = default;
	~GameUI() = default;
	void Init();
	void Submit(float timeLeft, uint32_t round, uint32_t teamCount, uint32_t activeTeam);
	void OnUpdate(Engine::Timestep ts);
};


typedef enum class InputDeviceType
{
	Unknown, Mouse, Keyboard, Gamepad,
}InputDevice;

class InputUI
{
private:
	bool m_Scene = false; // true for gameLayer, false for others.
	InputDeviceType m_DeviceType = InputDevice::Mouse;
	Engine::Ref<Engine::Texture2D> m_NormalMouseTex,
		m_NormalKeyTex, m_NormalGamepadTex, m_GamingMouseTex,
		m_GamingKeyTex, m_GamingGamepadTex;
private:
	static bool CheckGamepadInputState();
public:
	InputUI() = default;
	~InputUI() = default;

	static void Init();
	static void Submit(bool scene, InputDeviceType inputDeviceType);
	static void OnUpdate(Engine::Timestep ts);
	static void OnEvent(Engine::Event& e);

};
