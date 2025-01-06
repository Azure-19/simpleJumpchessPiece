#include "Menu.h"
#include "Effects/ButtonEffect.h"

#include "Entities/SettingController.h"
#include "Entities/UI.h"

constexpr std::array<int32_t, 1> m_LockList = { -1 };

void Menu::UpdateGamepadEvent(Engine::Timestep ts)
{
	if (m_ButtonChoice != -1)
		m_ButtonList[m_ButtonChoice]->OnUpdate(ts);
}

void Menu::OnAttach()
{
	// framebuffer init
	Engine::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Engine::Framebuffer::Create(fbSpec);

	// texture init
	{
		// origin
		m_BackgroundTexture = Engine::Texture2D::Create("assets/textures/Backgrounds/menuBackground_0.png");
		m_StartButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/startButton.png");
		m_SettingButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/settingButton.png");
		m_ControlButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/controlButton.png");
		m_TestButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/exitButton.png");
		//sub
		m_ExitButtonBase = Engine::SubTexture2D::CreateFromCoord(m_TestButtonTex, { 0, 1 }, { 150, 50 });
		m_ExitButtonCover = Engine::SubTexture2D::CreateFromCoord(m_TestButtonTex, { 0, 0 }, { 150, 50 });
	}

	// button init
	{
		m_ButtonList[0] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(75, 100), glm::vec2(225, 50));
			m_ExitButton = static_cast<Engine::LayerButton*>(m_ButtonList[0].get());

		m_ButtonList[1] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(75, 520), glm::vec2(225, 470));
			m_ControlButton = static_cast<Engine::LayerButton*>(m_ButtonList[1].get());

		m_ButtonList[2] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(75, 600), glm::vec2(225, 550));
			m_SettingButton = static_cast<Engine::LayerButton*>(m_ButtonList[2].get());

		m_ButtonList[3] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(75, 680), glm::vec2(225, 630));
			m_StartButton = static_cast<Engine::LayerButton*>(m_ButtonList[3].get());
		// Temporary: used in gameEngine building works
		m_ExitButton->SetLayer(MENU_LAYER, SANDBOX2D_LAYER);
		m_StartButton->SetLayer(MENU_LAYER, MODESELECT_LAYER);
		m_SettingButton->SetLayer(MENU_LAYER, SETTING_LAYER);
		m_ControlButton->SetLayer(MENU_LAYER, CONTROL_LAYER);
	}

	// sound source init
	{
		Engine::AudioPlayer2D::AddSource("buttonEffectSound", "assets/sounds/buttonPickedSound.wav");

		if (GameSetting::GetMenuBGMIndex())
		{
			Engine::AudioPlayer2D::AddSource("menuBGM_0", "assets/sounds/menuBGM_0.wav");
			Engine::AudioPlayer2D::RemoveSource("menuBGM_1");
		}
		else
		{
			Engine::AudioPlayer2D::AddSource("menuBGM_1", "assets/sounds/menuBGM_1.wav");
			Engine::AudioPlayer2D::RemoveSource("menuBGM_0");
		}
		// first play
		if (Engine::AudioPlayer2D::CheckSource(GameSetting::GetMenuBGMIndex() ? "menuBGM_0" : "menuBGM_1") != Engine::SourceState::Playing)
			Engine::AudioPlayer2D::PlaySource(GameSetting::GetMenuBGMIndex() ? "menuBGM_0" : "menuBGM_1",
				1.0f * GameSetting::GetGlobalGain() * GameSetting::GetGlobalMusicGain(), 1.0f, true);
	}

	InputUI::Init();
	m_ParticleSystem.DefaultInit(m_Particle);
}

void Menu::OnDetach()
{
}

void Menu::OnEnable()
{
	// check audio play work
	if (Engine::AudioPlayer2D::CheckSource(GameSetting::GetMenuBGMIndex() ? "menuBGM_0" : "menuBGM_1") != Engine::SourceState::Playing)
	{
		Engine::AudioPlayer2D::StopSource(GameSetting::GetGameBGMIndex() ? "gameBGM_0" : "gameBGM_1");
		Engine::AudioPlayer2D::PlaySource(GameSetting::GetMenuBGMIndex() ? "menuBGM_0" : "menuBGM_1",
			1.0f * GameSetting::GetGlobalGain() * GameSetting::GetGlobalMusicGain(), 1.0f, true);
	}
}

void Menu::OnDisable()
{
	m_ButtonChoice = -1;
}

void Menu::OnUpdate(Engine::Timestep ts)
{
	EG_PROFILE_FUNCTION();
	// update ButtonList
	ButtonEffect::UpdateButtonListD1(m_ButtonList, m_ButtonChoice, m_LockList);
	UpdateGamepadEvent(ts);
	// Disable for test Update
	// m_CameraController.OnUpdate(ts);

	// Render
	{
		EG_PROFILE_SCOPE("Render Prep");
		m_Framebuffer->Bind();
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.11f, 1.0f });
		Engine::RenderCommand::Clear();
	}
	{
		EG_PROFILE_SCOPE("Render Call");
		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Engine::Renderer2D::DrawCoord({ 0.0f, 720.0f }, { 1280.0f, 0.0f }, m_BackgroundTexture);

			ButtonEffect::UpdateButton(*m_ExitButton, m_ExitButtonCover, m_ExitButtonBase, "buttonEffectSound",
				0, m_ButtonChoice, EffectProps{ false, 1.0f });

			ButtonEffect::UpdateButton(*m_ControlButton, m_ControlButtonTex,
				1, m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));
			ButtonEffect::UpdateButton(*m_SettingButton, m_SettingButtonTex, 
				2, m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));
			ButtonEffect::UpdateButton(*m_StartButton, m_StartButtonTex,
				3, m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));

			InputUI::OnUpdate(ts);
		}
		Engine::Renderer2D::EndScene();

		if (GameSetting::GetParticleState())
			m_ParticleSystem.DefaultUpdate(ts, m_CameraController, m_Particle);

	}
}

void Menu::OnImGuiRender()
{
}

void Menu::OnEvent(Engine::Event& e)
{
	if (e.IsInCategory(Engine::EventCategory::EventCategoryMouse))
		Engine::Input::SetCursorHiddenState(false);

	m_StartButton->OnEvent(e);
	m_SettingButton->OnEvent(e);
	m_ControlButton->OnEvent(e);
	// Engine test
	m_ExitButton->OnEvent(e);

	InputUI::OnEvent(e);
}
