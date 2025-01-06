#include "PauseLayer.h"

#include "Effects/ButtonEffect.h"
#include "Entities/SettingController.h"
#include "Entities/UI.h"
// TEMP
#include "GameLayer.h"

constexpr std::array<int32_t, 1> m_LockList = { -1 };


void PauseLayer::UpdateGamepadEvent(Engine::Timestep ts)
{
	if (m_ButtonChoice != -1)
		m_ButtonList[m_ButtonChoice]->OnUpdate(ts);
	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonB))
		m_QuitButton->ShiftLayer();

	if (m_SaveButton->GetKeyChosenState() &&
		Engine::Input::IsKeyPressed(Engine::KeyCode::Z) ||
		Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonA))
	{
		if (m_ShakeFreeFlag)
		{
			GameLayer* gameLayer = static_cast<GameLayer*>(Engine::Application::Get().VisitLayer(GAME_LAYER));
			gameLayer->Save(m_SaveButton->GetFilepath());
			m_ShakeFreeFlag = false;
		}
	}
	else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Z) &&
		Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonA))
		m_ShakeFreeFlag = true;

	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonX) ||
		Engine::Input::IsKeyPressed(Engine::KeyCode::Escape) ||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::Button3))
	{
		m_QuitButton->ShiftLayer();
		m_ButtonChoice = -1;
	}

}


void PauseLayer::OnAttach()
{
	// texture init
	{
		// origin
		m_BackgroundTexture = 0 ? Engine::Texture2D::Create("assets/textures/Backgrounds/pauseBackground_1.png") : Engine::Texture2D::Create("assets/textures/Backgrounds/pauseBackground_0.jpg");
		m_QuitButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/quitButton.png");
		m_MenuButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/menuButton.png");
		m_SaveButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/saveButton.png");
	}
	// button init
	{
		m_ButtonList[0] = Engine::CreateRef<Engine::FileButton>(glm::vec2(565, 480), glm::vec2(715, 430), "save/quickSave.azur");
		m_SaveButton = static_cast<Engine::FileButton*>(m_ButtonList[0].get());

		m_ButtonList[1] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(565, 580), glm::vec2(715, 530));
		m_MenuButton = static_cast<Engine::LayerButton*>(m_ButtonList[1].get());

		m_ButtonList[2] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(565, 680), glm::vec2(715, 630));
		m_QuitButton = static_cast<Engine::LayerButton*>(m_ButtonList[2].get());
		// layer link
		m_QuitButton->SetLayer(PAUSE_LAYER, GAME_LAYER);
		m_MenuButton->SetLayer(PAUSE_LAYER, MENU_LAYER);
	}

	// particle init
	m_ParticleSystem.DefaultInit(m_Particle);
	Engine::AudioPlayer2D::AddSource("pauseBGM_0", "assets/sounds/pauseBGM_0.wav");
}

void PauseLayer::OnDetach()
{
}

void PauseLayer::OnEnable()
{

	Engine::AudioPlayer2D::PlaySource("pauseBGM_0",
		1.0f * GameSetting::GetGlobalGain() * GameSetting::GetGlobalMusicGain(), 1.0f, true);
}

void PauseLayer::OnDisable()
{
	Engine::AudioPlayer2D::StopSource("pauseBGM_0");
}

void PauseLayer::OnUpdate(Engine::Timestep ts)
{
	EG_PROFILE_FUNCTION();
	ButtonEffect::UpdateButtonListD1(m_ButtonList, m_ButtonChoice, m_LockList);
	UpdateGamepadEvent(ts);

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

			ButtonEffect::UpdateButton(*m_SaveButton, m_SaveButtonTex, 0, m_ButtonChoice, "buttonEffectSound");
			ButtonEffect::UpdateButton(*m_MenuButton, m_MenuButtonTex, 1, m_ButtonChoice, "buttonEffectSound");
			ButtonEffect::UpdateButton(*m_QuitButton, m_QuitButtonTex, 2, m_ButtonChoice, "buttonEffectSound");

			InputUI::OnUpdate(ts);
		}
		Engine::Renderer2D::EndScene();

		if (GameSetting::GetParticleState())
			m_ParticleSystem.DefaultUpdate(ts, m_CameraController, m_Particle);

	}	
}

void PauseLayer::OnImGuiRender()
{

}

void PauseLayer::OnEvent(Engine::Event& e)
{
	if (e.IsInCategory(Engine::EventCategory::EventCategoryMouse))
		Engine::Input::SetCursorHiddenState(false);
	else if (Engine::Input::IsKeyPressed(Engine::KeyCode::Escape))
	{
		m_QuitButton->ShiftLayer();
		m_ButtonChoice = -1;
	}

	m_QuitButton->OnEvent(e);
	m_MenuButton->OnEvent(e);

	if(m_SaveButton->IsMouseChosen() && Engine::Input::IsMouseButtonPressed(Engine::MouseCode::ButtonLeft))
	{
		GameLayer* gameLayer = static_cast<GameLayer*>(Engine::Application::Get().VisitLayer(GAME_LAYER));
		gameLayer->Save(m_SaveButton->GetFilepath());
	}
	InputUI::OnEvent(e);
}