#pragma once

#include <Engine.h>

class GameSetting
{
private:
	float GlobalGain = 1.0f;
	float GlobalEffectGain = 1.0f;
	float GlobalMusicGain = 0.1f;
	bool ParticleEnable = true;
	uint32_t MenuBGM = 1;
	uint32_t GameBGM = 1;
	bool MenuBGMChanged = false;
	bool GameBGMChanged = false;
public:
	GameSetting() = default;
	GameSetting(float globalGain, float effectGain, float musicGain, bool particleEnable, uint32_t menuBGM, uint32_t GameBGM);
	static GameSetting& Get();
	static float GetGlobalGain();
	static float GetGlobalEffectGain();
	static float GetGlobalMusicGain();
	static bool GetParticleState();
	static uint32_t GetMenuBGMIndex();
	static uint32_t GetGameBGMIndex();
	static void Save(const std::string& filepath);
	static void Load(const std::string& filepath);

	static void Upload(const GameSetting& settingData);
	static void Update();

};