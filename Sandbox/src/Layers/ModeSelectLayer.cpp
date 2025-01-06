#include "ModeSelectLayer.h"
#include "GameLayer.h"

#include "Effects/ButtonEffect.h"
#include "Entities/SettingController.h"
#include "Entities/UI.h"

constexpr std::array<glm::ivec2, 2> m_LockList = { glm::ivec2(0, 1), glm::ivec2(1, 1) };

void ModeSelectLayer::UpdateGamepadEvent(Engine::Timestep ts)
{
	if (m_ButtonChoice != glm::ivec2(-1))
		m_ButtonList[m_ButtonChoice.x][m_ButtonChoice.y]->OnUpdate(ts);
	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonX) ||
		Engine::Input::IsKeyPressed(Engine::KeyCode::Escape) ||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::Button3) &&
		m_ButtonChoice == glm::ivec2(-1))
			m_QuitButton->ShiftLayer();
}


void ModeSelectLayer::OnAttach()
{
	// texture init
	{
		//origin
		m_BackgroundTexture = Engine::Texture2D::Create("assets/textures/Backgrounds/modeSelectBackground_0.png");
		m_StartButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/startButton.png");
		m_QuitButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/quitButton.png");
		m_LoadButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/loadButton.png");

		m_ModeButtonTex[0] = Engine::Texture2D::Create("assets/textures/Buttons/modeSelectButton.png");
		m_ModeButtonTex[1] = Engine::Texture2D::Create("assets/textures/Buttons/modeSelectButtonOpt.png");
		m_TeamCountButtonTex[0] = Engine::Texture2D::Create("assets/textures/Buttons/switchButton.png");
		m_TeamCountButtonTex[1] = Engine::Texture2D::Create("assets/textures/Buttons/switchButtonOpt.png");
	}
	// button init
	{
		m_ButtonList[0][0] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(75, 100), glm::vec2(225, 50));
			m_QuitButton = static_cast<Engine::LayerButton*>(m_ButtonList[0][0].get());

		m_ButtonList[0][1] = Engine::CreateRef<Engine::SwitchButton>(glm::vec2(75, 500), glm::vec2(75 + 300, 500 - 150), 2);
			m_GameModeButton = static_cast<Engine::SwitchButton*>(m_ButtonList[0][1].get());

		m_ButtonList[1][1] = Engine::CreateRef<Engine::SwitchButton>(glm::vec2(515, 580), glm::vec2(765, 530), 5);
			m_TeamCountButton = static_cast<Engine::SwitchButton*>(m_ButtonList[1][1].get());
		//m_ButtonList[1][0] = nullptr; --[done in init]
		m_ButtonList[2][0] = Engine::CreateRef<Engine::FileButton>(glm::vec2(1025, 580), glm::vec2(1175, 530), "save/quickSave.azur");
			m_LoadButton = static_cast<Engine::FileButton*>(m_ButtonList[2][0].get());

		m_ButtonList[2][1] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(1025, 680), glm::vec2(1175, 630));
			m_StartButton = static_cast<Engine::LayerButton*>(m_ButtonList[2][1].get());


		m_StartButton->SetLayer(MODESELECT_LAYER, GAME_LAYER);
		m_QuitButton->SetLayer(MODESELECT_LAYER, MENU_LAYER);
		m_GameModeButton->SetOptRadius(75);
	}
	// particle init
	m_ParticleSystem.DefaultInit(m_Particle);

}

void ModeSelectLayer::OnDetach()
{
}

void ModeSelectLayer::OnEnable()
{
	GameLayer* gameLayer = static_cast<GameLayer*>(Engine::Application::Get().VisitLayer(GAME_LAYER));
	gameLayer->m_GameProps.Restart = true;
}

void ModeSelectLayer::OnDisable()
{
	m_ButtonChoice = { -1, -1 };
	// stop menuBGM
	Engine::AudioPlayer2D::RemoveSource(GameSetting::GetMenuBGMIndex() ? "menuBGM0" : "menuBGM1");

	// in case gamepad won't go through OnEvent. Upload state to gameLayer(disable first then enable)
	GameLayer* gameLayer = static_cast<GameLayer*>(Engine::Application::Get().VisitLayer(GAME_LAYER));
	gameLayer->m_GameProps.Load = m_LoadButton->CheckTrigger();
	gameLayer->m_GameProps.GameMode = m_GameModeButton->GetIndex();
	gameLayer->m_GameProps.LoadPath = m_LoadButton->GetFilepath();
	if (!gameLayer->m_GameProps.Load)
		gameLayer->m_GameProps.TeamCount = static_cast<TeamCount>(m_TeamCountButton->GetIndex());
}

void ModeSelectLayer::OnUpdate(Engine::Timestep ts)
{
	// update buttonList
	ButtonEffect::UpdateButtonListD2(m_ButtonList, m_ButtonChoice, m_LockList);
	UpdateGamepadEvent(ts);

	EG_PROFILE_FUNCTION();
	{
		EG_PROFILE_SCOPE("Render Prep");
		Engine::Renderer2D::ResetStats();
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.11f, 1.0f });
		Engine::RenderCommand::Clear();
	}
	{
		EG_PROFILE_SCOPE("Render Call");
		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Engine::Renderer2D::DrawCoord({ 0.0f, 720.0f }, { 1280.0f, 0.0f }, m_BackgroundTexture);

			ButtonEffect::UpdateButton(*m_LoadButton, m_LoadButtonTex, 
				glm::ivec2(2, 0), m_ButtonChoice, "buttonEffectSound", EffectProps(m_LoadButton->CheckTrigger() ? goldTintColor : orangeTintColor));
			ButtonEffect::UpdateButton(*m_StartButton, m_StartButtonTex,
				glm::ivec2(2, 1), m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));
			ButtonEffect::UpdateButton(*m_QuitButton, m_QuitButtonTex,
				glm::ivec2(0, 0), m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));

			ButtonEffect::UpdateToggleSwitchButton(*m_GameModeButton, m_ModeButtonTex[0], m_ModeButtonTex[1],
				glm::ivec2(0, 1), m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));
			ButtonEffect::UpdateToggleSwitchButton(*m_TeamCountButton, m_TeamCountButtonTex[0], m_TeamCountButtonTex[1],
				glm::ivec2(1, 1), m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));

			InputUI::OnUpdate(ts);
		}
		Engine::Renderer2D::EndScene();

		if (GameSetting::GetParticleState())
			m_ParticleSystem.DefaultUpdate(ts, m_CameraController, m_Particle);

	}
}

void ModeSelectLayer::OnEvent(Engine::Event& e)
{
	if (e.IsInCategory(Engine::EventCategory::EventCategoryMouse))
		Engine::Input::SetCursorHiddenState(false);
	else if (Engine::Input::IsKeyPressed(Engine::KeyCode::Escape))
		m_QuitButton->ShiftLayer();

	m_QuitButton->OnEvent(e);

	m_TeamCountButton->OnEvent(e);
	m_GameModeButton->OnEvent(e);
	m_LoadButton->OnEvent(e);
	if (m_LoadButton->IsMousePressed())
		m_LoadButton->CheckTrigger() ? m_LoadButton->InvalidateTrigger() : m_LoadButton->RecordTrigger();
	m_StartButton->OnEvent(e);


	InputUI::OnEvent(e);
}
