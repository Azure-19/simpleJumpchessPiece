#pragma once

#include <Engine.h>

#include "Core.h"

#include "Effects/ParticleSystem.h"

// we surely need text or combo box... ...
class ModeSelectLayer : public Engine::Layer
{
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Ref<Engine::Texture2D> m_BackgroundTexture,
		m_StartButtonTex, m_QuitButtonTex, m_LoadButtonTex;
	std::array<Engine::Ref<Engine::Texture2D>, 2> m_TeamCountButtonTex
		, m_ModeButtonTex;

	// D1:column, D2:raw
	std::array<std::array<Engine::Ref<Engine::Button>, 2>, 3> m_ButtonList = { nullptr };
	// nickNames
	Engine::LayerButton* m_StartButton,* m_QuitButton;
	Engine::SwitchButton* m_TeamCountButton,* m_GameModeButton;
	Engine::FileButton* m_LoadButton;
	glm::ivec2 m_ButtonChoice = { -1, -1 }; // (-1, -1) stands for no input

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
public:
	ModeSelectLayer();
	virtual ~ModeSelectLayer() = default;

	inline unsigned int GetSwitchButtonIndex() { return m_TeamCountButton->GetIndex(); }

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnEvent(Engine::Event& e) override;
};

