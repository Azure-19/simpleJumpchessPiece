#pragma once

#include <Engine.h>

#include "Core.h"

#include "Effects/ParticleSystem.h"
#include "Entities/SettingController.h"

class SettingLayer : public Engine::Layer
{
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Ref<Engine::Texture2D> m_BackgroundTexture, m_QuitButtonTex,
		m_SwitchButtonTex, m_MenuBGMButtonTex, m_GameBGMButtonTex, 
		m_ParticleEnableButtonTex;
	Engine::Ref<Engine::SubTexture2D> m_SwitchButtonOption, m_SwitchButtonLeft,
		m_SwitchButtonRight, m_MenuBGMButtonTex0, m_GameBGMButtonTex0, 
		m_ParticleEnableButtonTex0, m_MenuBGMButtonTex1, m_GameBGMButtonTex1, 
		m_ParticleEnableButtonTex1;
	// D1:column, D2:raw
	std::array<std::array<Engine::Ref<Engine::Button>, 6>, 2> m_ButtonList = { nullptr };
	// nickNames
	Engine::LayerButton* m_QuitButton;
	Engine::ChoseButton* m_MenuBGMButton, *m_GameBGMButton, *m_ParticleEnableButton;
	Engine::SwitchButton* m_GlobalSoundButton,* m_EffectSoundButton,* m_MusicSoundButton;
	glm::ivec2 m_ButtonChoice = { -1, -1 };

	// config simple particle system 
	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
public:
	SettingLayer();
	virtual ~SettingLayer() = default;
	// behave as a constructor++ and the other destructor++
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& e) override;
};

