#include "WinLayer.h"
#include "Core.h"
#include "Effects/ButtonEffect.h"
#include "Entities/UI.h"

#include "GameLayer.h"

constexpr std::array<int32_t, 1> m_LockList = { -1 };

void WinLayer::UpdateGamepadEvent(Engine::Timestep ts)
{
	if (m_ButtonChoice != -1)
		m_ButtonList[m_ButtonChoice]->OnUpdate(ts);
	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonX) ||
		Engine::Input::IsKeyPressed(Engine::KeyCode::Escape)||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::Button3))
	{
		m_ButtonChoice = 0;
		m_QuitButton->ShiftLayer();
	}
}

void WinLayer::OnAttach()
{
	// texture init
	{
		m_BackgroundTexture = 1 ? Engine::Texture2D::Create("assets/textures/Backgrounds/winBackground_0.jpg"):
			Engine::Texture2D::Create("assets/textures/Backgrounds/winBackground_1.png");
		m_QuitButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/menuButton.png");
		m_WinUITex = Engine::Texture2D::Create("assets/textures/UI/winUI.png");
		m_TintQuad = Engine::Texture2D::Create("assets/textures/Basic/shadedRoundedRectangle.png");
	}
	// button init
	{
		m_ButtonList[0] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(565, 680), glm::vec2(715, 630));
			m_QuitButton = static_cast<Engine::LayerButton*>(m_ButtonList[0].get());

		m_QuitButton->SetLayer(WIN_LAYER, MENU_LAYER);
	}
}

void WinLayer::OnDetach()
{
}

void WinLayer::OnEnable()
{
	GameLayer* gameLayer = static_cast<GameLayer*>(Engine::Application::Get().VisitLayer(GAME_LAYER));
	m_Winner = gameLayer->m_GameProps.Winner;
	m_QuitButton->SetKeyChosenState(true);
}

void WinLayer::OnDisable()
{
	m_Winner = 0;
	GameLayer* gameLayer = static_cast<GameLayer*>(Engine::Application::Get().VisitLayer(GAME_LAYER));
	gameLayer->m_GameProps.Winner = 0;
	gameLayer->m_GameProps.Load = false;
	m_ButtonChoice = 0;
}

void WinLayer::OnUpdate(Engine::Timestep ts)
{
	constexpr uint32_t xMinorCoord = 440;
	constexpr uint32_t yMinorCoord = 300;
	constexpr uint32_t boardWidth = 400;
	constexpr uint32_t boardHeight = 300;

	constexpr uint32_t quadWidth = 200;
	constexpr uint32_t quadHeight = 140;
	constexpr uint32_t quadOffsetX = 100;
	constexpr uint32_t quadOffsetY = 100;

	ButtonEffect::UpdateButtonListD1(m_ButtonList, m_ButtonChoice, m_LockList);
	// clear last frame
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.11f, 1.0f });
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		Engine::Renderer2D::DrawCoord({ 0.0f, 720.0f }, { 1280.0f, 0.0f }, m_BackgroundTexture);
		Engine::Renderer2D::DrawCoord({ xMinorCoord, yMinorCoord + boardHeight },
			{ xMinorCoord + boardWidth, yMinorCoord }, m_WinUITex);

		ButtonEffect::UpdateButton(*m_QuitButton, m_QuitButtonTex, 0, m_ButtonChoice , "buttonEffectSound");
		InputUI::OnUpdate(ts);
	}
	Engine::Renderer2D::EndScene();

	constexpr glm::vec4 black = { 0.1f, 0.1f, 0.1f, 1.0f };
	constexpr glm::vec4 yellow = { 1.0f, 1.0f, 0.5f, 1.0f };
	constexpr glm::vec4 green = { 0.4f, 1.0f, 0.6f, 1.0f };
	constexpr glm::vec4 blue = { 0.4f, 0.6f, 1.0f, 1.0f };
	constexpr glm::vec4 purple = { 0.9f, 0.1f, 1.0f, 1.0f };
	constexpr glm::vec4 red = { 1.0f, 0.4f, 0.3f, 1.0f };

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	switch (m_Winner)
	{
	case 1:
		Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
			{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY  },
			m_TintQuad, 1.0f, black);
		break;
	case 2:
		Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
			{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY  },
			m_TintQuad, 1.0f, yellow);
		break;
	case 3:
		Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
			{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY  },
			m_TintQuad, 1.0f, green);
		break;
	case 4:
		Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
			{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY  },
			m_TintQuad, 1.0f, blue);
		break;
	case 5:
		Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
			{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY  },
			m_TintQuad, 1.0f, purple);
		break;
	case 6:
		Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
			{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY  },
			m_TintQuad, 1.0f, red);
		break;
	default:
		break;
	}
	Engine::Renderer2D::EndScene();

}

void WinLayer::OnEvent(Engine::Event& e)
{
	if (e.IsInCategory(Engine::EventCategory::EventCategoryMouse))
		Engine::Input::SetCursorHiddenState(false);

	m_QuitButton->OnEvent(e);

	InputUI::OnEvent(e);
}
