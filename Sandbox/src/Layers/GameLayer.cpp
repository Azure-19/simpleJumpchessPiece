#include "GameLayer.h"
#include "Effects/ButtonEffect.h"

#include "Entities/SettingController.h"

constexpr std::array<int32_t, 1> m_LockListD1 = { -1 }; // for right two button


void GameLayer::UpdateGamepadEvent(Engine::Timestep ts)
{
	if (m_ButtonChoice != -1)
		m_ButtonList[m_ButtonChoice]->OnUpdate(ts);
}

void GameLayer::ListenControlState()
{
	if (m_SkipButton->GetMousePickedState())
	{
		m_Chessboard.Skip();
		m_SkipButton->SetMousePickedState(false);
	}
	else if (m_SkipButton->GetKeyPickState())
	{
		m_Chessboard.Skip();
		m_SkipButton->SetKeyPickState(false);
	}
	else if (m_SkipButton->CheckTrigger()) // only used for change ControlState( C / RB )
	{
		m_Chessboard.SetControlState(!m_Chessboard.GetControlState());
		m_SkipButton->InvalidateTrigger();
	}
	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonStart))
		m_PauseButton->ShiftLayer();
	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonRB))
	{
		if (m_ShakeFreeFlag)
		{
			m_SkipButton->RecordTrigger();
			m_ShakeFreeFlag = false;
		}
	}
	else if (Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonRB))
		m_ShakeFreeFlag = true;
}


void GameLayer::OnAttach()
{
	// texture init
	{
		// origin
		m_BackgroundTexture = 1 ? Engine::Texture2D::Create("assets/textures/Backgrounds/gameBackground_0.jpg"):
			Engine::Texture2D::Create("assets/textures/Backgrounds/gameBackground_1.png");
		m_PlayerTexture = Engine::Texture2D::Create("assets/textures/KomijiSatori.png");
		m_SkipButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/skipButton.png");
		m_PauseButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/pauseButton.png");
	}
	{
		m_ButtonList[0] = Engine::CreateRef<Engine::Button>(glm::vec2(1075, 350), glm::vec2(1225, 300));
			m_SkipButton = static_cast<Engine::Button*>(m_ButtonList[0].get());

		m_ButtonList[1] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(1075, 420), glm::vec2(1225, 370));
			m_PauseButton = static_cast<Engine::LayerButton*>(m_ButtonList[1].get());

		m_PauseButton->SetLayer(GAME_LAYER, PAUSE_LAYER);
	}
	// particle init
	m_ParticleSystem.DefaultInit(m_Particle);

}

void GameLayer::OnDetach()
{
}

void GameLayer::OnEnable()
{
	if(m_GameProps.Restart)
	{
		m_Chessboard.Init(m_GameProps.TeamCount, m_GameProps.GameMode, m_GameProps.Load, m_GameProps.LoadPath);
		m_GameProps.Restart = false;
	}
	// gameBGM init
	{
		Engine::AudioPlayer2D::StopSource(GameSetting::GetMenuBGMIndex() ? "menuBGM_0" : "menuBGM_1");
		if (GameSetting::GetGameBGMIndex())
		{
			Engine::AudioPlayer2D::AddSource("gameBGM_0", "assets/sounds/gameBGM_0.wav");
			Engine::AudioPlayer2D::RemoveSource("gameBGM_1");
		}
		else
		{
			Engine::AudioPlayer2D::AddSource("gameBGM_1", "assets/sounds/gameBGM_1.wav");
			Engine::AudioPlayer2D::RemoveSource("gameBGM_0");
		}
	}
	Engine::AudioPlayer2D::PlaySource(GameSetting::GetGameBGMIndex() ? "gameBGM_0" : "gameBGM_1",
		1.0f * GameSetting::GetGlobalGain() * GameSetting::GetGlobalMusicGain(), 1.0f, true);
	InputUI::Submit(true, InputDevice::Unknown);

}

void GameLayer::OnDisable()
{
	Engine::AudioPlayer2D::PauseSource(GameSetting::GetGameBGMIndex() ? "gameBGM_0" : "gameBGM_1");
	InputUI::Submit(false, InputDevice::Unknown);
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	if(!m_Chessboard.GetControlState())
	{
		ButtonEffect::UpdateButtonListD1(m_ButtonList, m_ButtonChoice, m_LockListD1);
		UpdateGamepadEvent(ts);
	}
	ListenControlState();

	// check winning state
	m_GameProps.Winner = m_Chessboard.m_Props.Winner;
	if (m_GameProps.Winner)
		Engine::Application::Get().ShiftLayer(GAME_LAYER, WIN_LAYER);

	// Render
	{
		EG_PROFILE_SCOPE("Render Prep");
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.11f, 1.0f });
		Engine::RenderCommand::Clear();
	}
	{
		EG_PROFILE_SCOPE("Render Call");
		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Engine::Renderer2D::DrawCoord({ 0.0f, 720.0f }, { 1280.0f, 0.0f }, m_BackgroundTexture);
			Engine::Renderer2D::DrawCoord({ 900.0f, 720.0f }, { 1220.0f, 267.0f }, m_PlayerTexture);
			Engine::Renderer2D::DrawCoord(m_Chessboard.GetCoord()[0], m_Chessboard.GetCoord()[1], m_Chessboard.GetTexture());
			m_Chessboard.OnUpdate(ts);

			if(!m_Chessboard.GetControlState())
			{
				ButtonEffect::UpdateButton(*m_SkipButton, m_SkipButtonTex, 0, m_ButtonChoice, "buttonEffectSound");
				ButtonEffect::UpdateButton(*m_PauseButton, m_PauseButtonTex, 1, m_ButtonChoice, "buttonEffectSound");
			}
			InputUI::OnUpdate(ts);
		}
		Engine::Renderer2D::EndScene();
		
		if (GameSetting::GetParticleState())
			m_ParticleSystem.DefaultUpdate(ts, m_CameraController, m_Particle);
	}
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(Engine::Event& e)
{
	m_Chessboard.OnEvent(e);
	m_PauseButton->OnEvent(e);
	m_SkipButton->OnEvent(e);
	if (Engine::Input::IsKeyPressed(Engine::Key::C))
		m_SkipButton->RecordTrigger();
	if (e.GetEventType() == Engine::EventType::MouseMoved) // a really useful method which I realize it so late
		m_Chessboard.SetControlState(false);
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::Space)||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::ButtonMiddle))
		m_PauseButton->ShiftLayer();

	InputUI::OnEvent(e);
}


void GameLayer::Save(std::string& filepath)
{
	// Not expanded
	EG_PROFILE_FUNCTION();
	m_Chessboard.Save(filepath);
}

void GameLayer::Load(std::string& filepath)
{
	EG_PROFILE_FUNCTION();
	m_Chessboard.Load(filepath);
}
