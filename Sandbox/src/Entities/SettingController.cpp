#pragma once

#include "Entities/SettingController.h"

static GameSetting s_Instance;

GameSetting::GameSetting(float globalGain, float effectGain, float musicGain, bool particleEnable, uint32_t menuBGM, uint32_t gameBGM)
	:GlobalGain(globalGain), GlobalEffectGain(effectGain), GlobalMusicGain(musicGain), ParticleEnable(particleEnable), MenuBGM(menuBGM), GameBGM(gameBGM)
{
}

GameSetting& GameSetting::Get()
{
	return s_Instance;
}

float GameSetting::GetGlobalGain()
{
	return s_Instance.GlobalGain;
}

float GameSetting::GetGlobalEffectGain()
{
	return s_Instance.GlobalEffectGain;
}

float GameSetting::GetGlobalMusicGain()
{
	return s_Instance.GlobalMusicGain;
}

bool GameSetting::GetParticleState()
{
	return s_Instance.ParticleEnable;
}

uint32_t GameSetting::GetMenuBGMIndex()
{
	return s_Instance.MenuBGM;
}

uint32_t GameSetting::GetGameBGMIndex()
{
	return s_Instance.GameBGM;
}


constexpr uint32_t propsLength = 3 * sizeof(float) + 2 * sizeof(uint32_t) + sizeof(bool);
constexpr const char* loadType = "settingSave";

void GameSetting::Save(const std::string& filepath)
{
	size_t loadTypeLength = strlen(loadType);

	std::string saveData;
	saveData.resize(loadTypeLength + propsLength);
	uint32_t offset = 0;
	std::memcpy(&saveData[offset], loadType, loadTypeLength);
	offset += loadTypeLength;
	std::memcpy(&saveData[offset], &s_Instance.GlobalGain, sizeof(float));
	offset += sizeof(float);
	std::memcpy(&saveData[offset], &s_Instance.GlobalEffectGain, sizeof(float));
	offset += sizeof(float);
	std::memcpy(&saveData[offset], &s_Instance.GlobalMusicGain, sizeof(float));
	offset += sizeof(float);
	std::memcpy(&saveData[offset], &s_Instance.MenuBGM, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	std::memcpy(&saveData[offset], &s_Instance.GameBGM, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	std::memcpy(&saveData[offset], &s_Instance.ParticleEnable, sizeof(bool));

	// layout { float, float, float, uint32_t, uint32_t, bool }
	std::ofstream out(filepath, std::ios::out | std::ios::binary);
	if (out)
	{
		out.write(&saveData[0], saveData.size());
		if (!out)
		{
			EG_CORE_ERROR("Could not write file '{0}'", filepath);
		}
		out.close();
		EG_INFO("Setting saved in {0}.", filepath);
	}
	else
	{
		EG_CORE_ERROR("Could not open file '{0}'", filepath);
	}
}

void GameSetting::Load(const std::string& filepath)
{
	size_t loadTypeLength = strlen(loadType);

	std::string loadData;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);			// start from index 0 to end
		loadData.resize(in.tellg());		// adapt to file's size
		in.seekg(0, std::ios::beg);
		in.read(&loadData[0], loadData.size());
		in.close();
	}
	else
	{
		EG_ERROR("Could not open file '{0}'", filepath);
	}
	if (!loadData.empty())
	{
		size_t pos = loadData.find(loadType, 0);

		if (pos != std::string::npos)
		{
			uint32_t offset = 0;
			std::string settingData = loadData.substr(pos + loadTypeLength, propsLength);
			s_Instance.GlobalGain = *reinterpret_cast<float*>(&settingData[offset]);
			offset += sizeof(float);
			s_Instance.GlobalEffectGain = *reinterpret_cast<float*>(&settingData[offset]);
			offset += sizeof(float);
			s_Instance.GlobalMusicGain = *reinterpret_cast<float*>(&settingData[offset]);
			offset += sizeof(float);
			s_Instance.MenuBGM = *reinterpret_cast<uint32_t*>(&settingData[offset]);
			offset += sizeof(uint32_t);
			s_Instance.GameBGM = *reinterpret_cast<uint32_t*>(&settingData[offset]);
			offset += sizeof(uint32_t);
			s_Instance.ParticleEnable = static_cast<bool>(&settingData[offset]);

			EG_INFO("Setting loaded.");
		}
	}
	else
	{
		EG_INFO("Setting load FAILED.");
	}
}

void GameSetting::Upload(const GameSetting& settingData)
{
	s_Instance.GlobalGain = settingData.GlobalGain;
	s_Instance.GlobalEffectGain = settingData.GlobalEffectGain;
	s_Instance.GlobalMusicGain = settingData.GlobalMusicGain;
	s_Instance.ParticleEnable = settingData.ParticleEnable;
	if (settingData.MenuBGM != s_Instance.MenuBGM)
	{
		s_Instance.MenuBGMChanged = true;
		s_Instance.GameBGMChanged = false;
		s_Instance.MenuBGM = settingData.MenuBGM;
	}
	else if (settingData.GameBGM != s_Instance.GameBGM)
	{
		s_Instance.GameBGMChanged = true;
		s_Instance.MenuBGMChanged = false;
		s_Instance.GameBGM = settingData.GameBGM;
	}
	else if (s_Instance.MenuBGMChanged + s_Instance.GameBGMChanged)
	{
		s_Instance.MenuBGMChanged = false;
		s_Instance.GameBGMChanged = false;
	}
}

void GameSetting::Update()
{
	Engine::AudioPlayer2D::SetGlobalAttribute(s_Instance.GlobalGain, s_Instance.GlobalEffectGain, s_Instance.GlobalMusicGain);

	// enable client preview music
	if (s_Instance.MenuBGMChanged)
	{
		Engine::AudioPlayer2D::StopSource(GameSetting::GetMenuBGMIndex() ? "menuBGM_1" : "menuBGM_0");
		Engine::AudioPlayer2D::StopSource(GameSetting::GetGameBGMIndex() ? "gameBGM_0" : "gameBGM_1");
		Engine::AudioPlayer2D::PlaySource(GameSetting::GetMenuBGMIndex() ? "menuBGM_0" : "menuBGM_1",
				1.0f * GameSetting::GetGlobalGain() * GameSetting::GetGlobalMusicGain(), 1.0f, true);
	}
	else if (s_Instance.GameBGMChanged)
	{
		Engine::AudioPlayer2D::StopSource(GameSetting::GetGameBGMIndex() ? "gameBGM_1" : "gameBGM_0");
		Engine::AudioPlayer2D::StopSource(GameSetting::GetMenuBGMIndex() ? "menuBGM_0" : "menuBGM_1");
		Engine::AudioPlayer2D::PlaySource(GameSetting::GetGameBGMIndex() ? "gameBGM_0" : "gameBGM_1",
			1.0f * GameSetting::GetGlobalGain() * GameSetting::GetGlobalMusicGain(), 1.0f, true);
	}
}
