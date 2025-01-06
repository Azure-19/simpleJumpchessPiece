#include "SettingLayer.h"

#include "Effects/ButtonEffect.h"
#include "Entities/UI.h"

constexpr std::array<glm::ivec2, 3> m_LockList = { glm::ivec2(1, 3), glm::ivec2(1, 4), glm::ivec2(1, 5) };

void SettingLayer::UpdateGamepadEvent(Engine::Timestep ts)
{
	if(m_ButtonChoice != glm::ivec2(-1))
		m_ButtonList[m_ButtonChoice.x][m_ButtonChoice.y]->OnUpdate(ts);
	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonX) ||
		Engine::Input::IsKeyPressed(Engine::KeyCode::Escape) ||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::Button3))
	{
		m_QuitButton->ShiftLayer();
		m_ButtonChoice = glm::ivec2(-1, -1);
	}
}

void SettingLayer::OnAttach()
{
	// texture init
	{
	// origin
		m_BackgroundTexture = Engine::Texture2D::Create("assets/textures/Backgrounds/settingBackground_0.png");
		m_QuitButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/quitButton.png");
		m_SwitchButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/newSwitchButton.png");
		m_MenuBGMButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/MenuBGMButton.png");
		m_GameBGMButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/GameBGMButton.png");
		m_ParticleEnableButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/ParticleEnableButton.png");
	// sub
		m_MenuBGMButtonTex0 = Engine::SubTexture2D::CreateFromCoord(m_MenuBGMButtonTex, { 0, 1 }, { 200, 80 });
		m_MenuBGMButtonTex1 = Engine::SubTexture2D::CreateFromCoord(m_MenuBGMButtonTex, { 0, 0 }, { 200, 80 });
		m_GameBGMButtonTex0 = Engine::SubTexture2D::CreateFromCoord(m_GameBGMButtonTex, { 0, 1 }, { 200, 80 });
		m_GameBGMButtonTex1 = Engine::SubTexture2D::CreateFromCoord(m_GameBGMButtonTex, { 0, 0 }, { 200, 80 });
		m_ParticleEnableButtonTex0 = Engine::SubTexture2D::CreateFromCoord(m_ParticleEnableButtonTex, { 0, 1 }, { 200, 80 });
		m_ParticleEnableButtonTex1 = Engine::SubTexture2D::CreateFromCoord(m_ParticleEnableButtonTex, { 0, 0 }, { 200, 80 });

		m_SwitchButtonOption = Engine::SubTexture2D::CreateFromCoord(m_SwitchButtonTex, { 0, 1 }, { 50, 50 });
		m_SwitchButtonLeft = Engine::SubTexture2D::CreateFromCoord(m_SwitchButtonTex, { 0, 2 }, { 400, 50 });
		m_SwitchButtonRight = Engine::SubTexture2D::CreateFromCoord(m_SwitchButtonTex, { 0, 0 }, { 400, 50 });

	}
	// button init
	{
		m_ButtonList[0][0] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(75, 100), glm::vec2(225, 50));
			m_QuitButton = static_cast<Engine::LayerButton*>(m_ButtonList[0][0].get());
		// [0][i] are nullptr(no button)
		m_ButtonList[1][0] = Engine::CreateRef<Engine::ChoseButton>(glm::vec2(540, 200), glm::vec2(740, 120));
			m_GameBGMButton = static_cast<Engine::ChoseButton*>(m_ButtonList[1][0].get());

		m_ButtonList[1][1] = Engine::CreateRef<Engine::ChoseButton>(glm::vec2(540, 300), glm::vec2(740, 220));
			m_MenuBGMButton = static_cast<Engine::ChoseButton*>(m_ButtonList[1][1].get());

		m_ButtonList[1][2] = Engine::CreateRef<Engine::ChoseButton>(glm::vec2(540, 400), glm::vec2(740, 320));
			m_ParticleEnableButton = static_cast<Engine::ChoseButton*>(m_ButtonList[1][2].get());
		
		m_ButtonList[1][3] = Engine::CreateRef<Engine::SwitchButton>(glm::vec2(440, 480), glm::vec2(840, 430), 100, 10);
			m_GlobalSoundButton = static_cast<Engine::SwitchButton*>(m_ButtonList[1][3].get());
		
		m_ButtonList[1][4] = Engine::CreateRef<Engine::SwitchButton>(glm::vec2(440, 580), glm::vec2(840, 530), 100, 100);
			m_EffectSoundButton = static_cast<Engine::SwitchButton*>(m_ButtonList[1][4].get());

		m_ButtonList[1][5] = Engine::CreateRef<Engine::SwitchButton>(glm::vec2(440, 680), glm::vec2(840, 630), 100, 100);
			m_MusicSoundButton = static_cast<Engine::SwitchButton*>(m_ButtonList[1][5].get());
		// layer link
		m_QuitButton->SetLayer(SETTING_LAYER, MENU_LAYER);
	}
	// particle init
	m_ParticleSystem.DefaultInit(m_Particle);
	GameSetting::Load("save/settingSave.azur");
}

void SettingLayer::OnDetach()
{
}

void SettingLayer::OnEnable()
{
	// for preview
	Engine::AudioPlayer2D::AddSource("menuBGM_0", "assets/sounds/menuBGM_0.wav");
	Engine::AudioPlayer2D::AddSource("menuBGM_1", "assets/sounds/menuBGM_1.wav");
	Engine::AudioPlayer2D::AddSource("gameBGM_0", "assets/sounds/gameBGM_0.wav");
	Engine::AudioPlayer2D::AddSource("gameBGM_1", "assets/sounds/gameBGM_1.wav");
	// load setting(some tiny question exists)
	{
		m_GlobalSoundButton->SetIndex(GameSetting::GetGlobalGain() * 100.0f);
		m_EffectSoundButton->SetIndex(GameSetting::GetGlobalEffectGain() * 100.0f);
		m_MusicSoundButton->SetIndex(GameSetting::GetGlobalMusicGain() * 100.0f);
		m_ParticleEnableButton->SetChoice(GameSetting::GetParticleState());
		m_MenuBGMButton->SetChoice(GameSetting::GetMenuBGMIndex());
		m_GameBGMButton->SetChoice(GameSetting::GetGameBGMIndex());
	}
}

void SettingLayer::OnDisable()
{
	Engine::AudioPlayer2D::RemoveSource(GameSetting::GetMenuBGMIndex() ? "menuBGM_1" : "menuBGM_0");
	Engine::AudioPlayer2D::RemoveSource(GameSetting::GetGameBGMIndex() ? "gameBGM_1" : "gameBGM_0");
	GameSetting::Save("save/settingSave.azur");
}


void SettingLayer::OnUpdate(Engine::Timestep ts)
{
	EG_PROFILE_FUNCTION();
	// update buttonList
	ButtonEffect::UpdateButtonListD2(m_ButtonList, m_ButtonChoice, m_LockList);
	UpdateGamepadEvent(ts);

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
			// check the parameter later
			ButtonEffect::UpdateButton(*m_QuitButton, m_QuitButtonTex,
				glm::ivec2(0, 0), m_ButtonChoice, "buttonEffectSound", EffectProps(orangeTintColor));

			ButtonEffect::UpdateChoseButton(*m_GameBGMButton, m_GameBGMButtonTex0, m_GameBGMButtonTex1,
				glm::ivec2(1, 0), m_ButtonChoice, "buttonEffectSound");
			ButtonEffect::UpdateChoseButton(*m_MenuBGMButton, m_MenuBGMButtonTex0, m_MenuBGMButtonTex1,
				glm::ivec2(1, 1), m_ButtonChoice, "buttonEffectSound");
			ButtonEffect::UpdateChoseButton(*m_ParticleEnableButton, m_ParticleEnableButtonTex0, m_ParticleEnableButtonTex1,
				glm::ivec2(1, 2), m_ButtonChoice, "buttonEffectSound");
			ButtonEffect::UpdateLinearSwitchButton(*m_GlobalSoundButton, m_SwitchButtonOption, m_SwitchButtonLeft, m_SwitchButtonRight,
				glm::ivec2(1, 3), m_ButtonChoice, ts, "buttonEffectSound", EffectProps(whiteTintColor));
			ButtonEffect::UpdateLinearSwitchButton(*m_EffectSoundButton, m_SwitchButtonOption, m_SwitchButtonLeft, m_SwitchButtonRight, 
				glm::ivec2(1, 4), m_ButtonChoice, ts, "buttonEffectSound", EffectProps(whiteTintColor));
			ButtonEffect::UpdateLinearSwitchButton(*m_MusicSoundButton, m_SwitchButtonOption, m_SwitchButtonLeft, m_SwitchButtonRight,
				glm::ivec2(1, 5), m_ButtonChoice, ts, "buttonEffectSound", EffectProps(whiteTintColor));

			InputUI::OnUpdate(ts);
		}
		Engine::Renderer2D::EndScene();

		GameSetting::Upload({ m_GlobalSoundButton->GetIndex() / 100.0f,
			m_EffectSoundButton->GetIndex() / 100.0f,
			m_MusicSoundButton->GetIndex() / 100.0f,
			m_ParticleEnableButton->GetChoice(),
			m_MenuBGMButton->GetChoice(),
			m_GameBGMButton->GetChoice(),
			});
		GameSetting::Update();
		if (GameSetting::GetParticleState())
			m_ParticleSystem.DefaultUpdate(ts, m_CameraController, m_Particle);
	}
}

void SettingLayer::OnImGuiRender()
{

}

void SettingLayer::OnEvent(Engine::Event& e)
{
	if (e.IsInCategory(Engine::EventCategory::EventCategoryMouse))
		Engine::Input::SetCursorHiddenState(false);
	else if (Engine::Input::IsKeyPressed(Engine::KeyCode::Escape))
		m_QuitButton->ShiftLayer();

	m_QuitButton->OnEvent(e);
	m_ParticleEnableButton->OnEvent(e);
	m_MenuBGMButton->OnEvent(e);
	m_GameBGMButton->OnEvent(e);
	m_GlobalSoundButton->OnEvent(e);
	m_EffectSoundButton->OnEvent(e);
	m_MusicSoundButton->OnEvent(e);


	InputUI::OnEvent(e);
}
