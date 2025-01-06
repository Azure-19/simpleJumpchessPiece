#pragma once

#include <Engine.h>

#include "Core.h"
#include "Entities/Chessboard.h"

#include "Effects/ParticleSystem.h"

struct GameProps
{
	TeamCount TeamCount;
	bool Load = false;
	bool GameMode = false; // false is what?
	bool Restart = true;
	std::string LoadPath;
	uint32_t Winner = 0; // in chessboard
};

// index 2
class GameLayer : public Engine::Layer
{
private:
	Engine::OrthographicCameraController m_CameraController;

	Engine::Ref<Engine::Texture2D> m_BackgroundTexture, m_PlayerTexture,
		m_SkipButtonTex, m_PauseButtonTex;

	std::array<Engine::Ref<Engine::Button>, 2> m_ButtonList = { nullptr };
	// nickName
	Engine::LayerButton* m_PauseButton;
	Engine::Button* m_SkipButton;
	int32_t m_ButtonChoice = -1;
	bool m_ShakeFreeFlag = true;

	glm::vec4 m_SquareColor = { 0.9f, 0.3f, 0.8f, 1.0f };
	// TEMPORARY Below
	Chessboard m_Chessboard;
	// config simple particle system 
	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
public:
	//mode Select
	GameProps m_GameProps;
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
	void ListenControlState();
public:
	GameLayer();
	virtual ~GameLayer() = default;
	// behave as a constructor++ and the other destructor++
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& e) override;

	//TEMP
	void Save(std::string& filepath);
	void Load(std::string& filepath);
};

