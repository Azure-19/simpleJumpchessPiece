#pragma once

#include <Engine.h>

#include "Core.h"

#include "Effects/ParticleSystem.h"

class PauseLayer : public Engine::Layer
{
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Ref<Engine::Texture2D> m_BackgroundTexture, 
		m_QuitButtonTex, m_MenuButtonTex, m_SaveButtonTex;

	std::array<Engine::Ref<Engine::Button>, 3> m_ButtonList = { nullptr };
	// nickNames
	Engine::LayerButton* m_QuitButton,* m_MenuButton;
	Engine::FileButton* m_SaveButton;
	int32_t m_ButtonChoice = -1;
	bool m_ShakeFreeFlag = true;

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
public:
	PauseLayer();
	virtual ~PauseLayer() = default;
	// behave as a constructor++ and the other destructor++
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& e) override;
};

