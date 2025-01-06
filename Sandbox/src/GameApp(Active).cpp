#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Layers/Menu.h"
#include "Layers/GameLayer.h"
#include "Layers/SettingLayer.h"
#include "Layers/PauseLayer.h"
#include "Layers/ModeSelectLayer.h"
#include "Layers/WinLayer.h"
#include "Layers/ControlLayer.h"

#include "Sandbox2D.h"

// running in single layer mode now, a static m_Class results in link error(not knowing why)


Menu::Menu()
	: Layer("Menu", true), m_CameraController(1280.0f / 720.0f),
	m_ExitButton(nullptr), m_SettingButton(nullptr), m_StartButton(nullptr),
	m_ControlButton(nullptr)
{

}

GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(1280.0f / 720.0f),
	m_PauseButton(nullptr), m_SkipButton(nullptr)
{

}

SettingLayer::SettingLayer()
	: Layer("SettingLayer"), m_CameraController(1280.0f / 720.0f),
	m_QuitButton(nullptr), m_GameBGMButton(nullptr), m_MenuBGMButton(nullptr),
	m_ParticleEnableButton(nullptr), m_GlobalSoundButton(nullptr),
	m_EffectSoundButton(nullptr), m_MusicSoundButton(nullptr)
{

}

PauseLayer::PauseLayer()
	: Layer("PauseLayer"), m_CameraController(1280.0f / 720.0f),
	m_QuitButton(nullptr), m_MenuButton(nullptr), m_SaveButton(nullptr)
{

}

ModeSelectLayer::ModeSelectLayer()
	: Layer("ModeSelectLayer"), m_CameraController(1280.0f / 720.0f),
	m_QuitButton(nullptr), m_LoadButton(nullptr), m_StartButton(nullptr),
	m_TeamCountButton(nullptr), m_GameModeButton(nullptr)
{

}

WinLayer::WinLayer()
	: Layer("WinLayer"), m_CameraController(1280.0f / 720.0f),
	m_QuitButton(nullptr)
{
}

ControlLayer::ControlLayer()
	:Layer("ControlLayer"), m_CameraController(1280.0f / 720.0f),
	m_QuitButton(nullptr)
{
}

// test layer
Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}



class GameApp : public Engine::Application
{
public:
	GameApp()
	{
		PushLayer(new Sandbox2D);
		// update layer index in core.h
		PushLayer(new ControlLayer);
		PushLayer(new WinLayer);
		PushLayer(new ModeSelectLayer);
		PushLayer(new PauseLayer);
		PushLayer(new SettingLayer);
		PushLayer(new GameLayer);
		PushLayer(new Menu);
		
	}
	~GameApp()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new GameApp;
}
