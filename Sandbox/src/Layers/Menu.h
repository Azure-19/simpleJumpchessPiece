#pragma once

#include <Engine.h>

#include "Core.h"


#include "Effects/ParticleSystem.h"

class Menu : public Engine::Layer
{
private:
	Engine::OrthographicCameraController m_CameraController;
	Engine::Ref<Engine::Framebuffer> m_Framebuffer;

	Engine::Ref<Engine::Texture2D> m_BackgroundTexture,
		m_StartButtonTex, m_SettingButtonTex,
		m_ControlButtonTex ,m_TestButtonTex;

	Engine::Ref<Engine::SubTexture2D> m_ExitButtonBase,
		m_ExitButtonCover;

	std::array<Engine::Ref<Engine::Button>, 4> m_ButtonList = { nullptr };
	// to keep nickNames. Auto destruct is enough
	Engine::LayerButton* m_ExitButton,* m_SettingButton, * m_StartButton, *m_ControlButton;
	int32_t m_ButtonChoice = -1; // should reset to -1 after mouse input
	// bool m_ShakeFreeFlag = true; // in this layer no shake free is ok(just two state)

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
public:
	Menu();
	virtual ~Menu() = default;
	// behave as a constructor++ and the other destructor++
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& e) override;
};
