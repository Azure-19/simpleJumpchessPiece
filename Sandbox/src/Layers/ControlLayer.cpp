#include "ControlLayer.h"
#include "Core.h"

#include "Effects/ButtonEffect.h"

constexpr std::array<int32_t, 1> m_LockList = { -1 };

void ControlLayer::UpdateGamepadEvent(Engine::Timestep ts)
{
	
	if (m_ButtonChoice != -1)
		m_ButtonList[m_ButtonChoice]->OnUpdate(ts);
	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonX) ||
		Engine::Input::IsKeyPressed(Engine::KeyCode::Escape) ||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::Button3))
	{
		m_ButtonChoice = 0;
		m_QuitButton->ShiftLayer();
	}
	
}

void ControlLayer::OnAttach()
{
	// texture init
	{
		// crash here
		m_BackgroundTexture = 0 ? Engine::Texture2D::Create("assets/textures/Backgrounds/controlBackground_0.png") :
			Engine::Texture2D::Create("assets/textures/Backgrounds/controlBackground_1.png");
		m_QuitButtonTex = Engine::Texture2D::Create("assets/textures/Buttons/menuButton.png");
	}
	// button init
	{
		m_ButtonList[0] = Engine::CreateRef<Engine::LayerButton>(glm::vec2(565, 680), glm::vec2(715, 630));
		m_QuitButton = static_cast<Engine::LayerButton*>(m_ButtonList[0].get());

		m_QuitButton->SetLayer(CONTROL_LAYER, MENU_LAYER);
	}
	
}

void ControlLayer::OnDetach()
{
}

void ControlLayer::OnEnable()
{
}

void ControlLayer::OnDisable()
{
}

void ControlLayer::OnUpdate(Engine::Timestep ts)
{
	
	ButtonEffect::UpdateButtonListD1(m_ButtonList, m_ButtonChoice, m_LockList);
	UpdateGamepadEvent(ts);

	// clear last frame
	Engine::RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.11f, 1.0f });
	Engine::RenderCommand::Clear();
	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		Engine::Renderer2D::DrawCoord({ 0.0f, 720.0f }, { 1280.0f, 0.0f }, m_BackgroundTexture);
		ButtonEffect::UpdateButton(*m_QuitButton, m_QuitButtonTex, 0, m_ButtonChoice, "buttonEffectSound");
	}
	Engine::Renderer2D::EndScene();
	
}

void ControlLayer::OnEvent(Engine::Event& e)
{
	
	if (e.IsInCategory(Engine::EventCategory::EventCategoryMouse))
		Engine::Input::SetCursorHiddenState(false);

	m_QuitButton->OnEvent(e);
	
}
